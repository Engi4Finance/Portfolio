/*
 * ESP32 Autopilot Boat
 * Contrôle automatique de la direction d'un bateau via WiFi et capteur magnétomètre (HMC5883)
 * Interface web pour piloter le moteur et la direction.
 *
 * Auteur : Lionel FRITZ
 * Dernière mise à jour : 2024
 *
 * Dépendances Arduino :
 *   - WiFi.h
 *   - Servo.h
 *   - Adafruit HMC5883_U
 *   - Adafruit Sensor
 *
 * Sécurité :
 *   - Ne pas exposer ce code en production avec des identifiants WiFi en clair !
 */

// === CONFIGURATION ===
#include <WiFi.h>
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

const char* ssid = "POCOX3Pro";      // SSID WiFi
const char* password = "Fribri58";   // Mot de passe WiFi
const int servoPin = 13;              // Pin du servo moteur
const int IN1 = 26;                   // Pin du H-bridge (moteur)

// === OBJETS & VARIABLES ===
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
Servo myservo;
WiFiServer server(80);
String header;
String valueString;
String INState = "stopped";
int pos1 = 0, pos2 = 0;
float gap = 0;
int servoposition = 90;
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

// === SETUP ===
void setup() {
  Serial.begin(115200);
  myservo.attach(servoPin);
  pinMode(IN1, OUTPUT); digitalWrite(IN1, LOW);
  // Initialisation du magnétomètre
  if(!mag.begin()) {
    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
    while(1);
  }
  // Connexion WiFi
  Serial.print("Connexion à "); Serial.println(ssid);
  WiFi.begin(ssid, password);
  int wifi_attempts = 0;
  while (WiFi.status() != WL_CONNECTED && wifi_attempts < 20) {
    delay(500); Serial.print("."); wifi_attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connecté. IP : ");
    Serial.println(WiFi.localIP());
    server.begin();
  } else {
    Serial.println("\nErreur de connexion WiFi !");
  }
}

// === LOOP PRINCIPALE ===
void loop(){
  WiFiClient client = server.available();
  // Lecture du capteur
  sensors_event_t event; mag.getEvent(&event);
  float heading = atan2(event.magnetic.y, event.magnetic.x);
  float declinationAngle = 1.22; // Toulouse
  heading += declinationAngle;
  if(heading < 0) heading += 2*PI;
  if(heading > 2*PI) heading -= 2*PI;
  float headingDegrees = heading * 180/M_PI;
  Serial.print("mag: "); Serial.println(headingDegrees);
  gap = valueString.toInt() - headingDegrees;
  Serial.print("slider: "); Serial.println(valueString.toInt());
  if (gap > 180) gap = gap - 360;
  if (gap < -180) gap = gap + 360;
  // Logique de direction
  if (gap <= 2 && gap >= -2) servoposition = 90;
  else if (gap > 2 && gap <= 20) servoposition = 95;
  else if (gap > 20 && gap <= 40) servoposition = 100;
  else if (gap > 40 && gap <= 60) servoposition = 105;
  else if (gap > 60 && gap <= 80) servoposition = 110;
  else if (gap > 80 && gap <= 100) servoposition = 115;
  else if (gap > 100 && gap <= 120) servoposition = 120;
  else if (gap > 120 && gap <= 140) servoposition = 125;
  else if (gap > 140 && gap <= 160) servoposition = 130;
  else if (gap > 160 && gap <= 180) servoposition = 135;
  else if (gap < -2 && gap >= -20) servoposition = 85;
  else if (gap < -20 && gap >= -40) servoposition = 80;
  else if (gap < -40 && gap >= -60) servoposition = 75;
  else if (gap < -60 && gap >= -80) servoposition = 70;
  else if (gap < -80 && gap >= -100) servoposition = 65;
  else if (gap < -100 && gap >= -120) servoposition = 60;
  else if (gap < -120 && gap >= -140) servoposition = 55;
  else if (gap < -140 && gap >= -160) servoposition = 50;
  else servoposition = 45;
  myservo.write(servoposition);
  // === SERVEUR WEB ===
  if (client) {
    currentTime = millis(); previousTime = currentTime;
    Serial.println("Nouveau client.");
    String currentLine = "";
    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      currentTime = millis();
      if (client.available()) {
        char c = client.read(); Serial.write(c); header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            // Moteur
            if (header.indexOf("GET /IN/run") >= 0) { INState = "running"; digitalWrite(IN1, HIGH); }
            else if (header.indexOf("GET /IN/stop") >= 0) { INState = "stopped"; digitalWrite(IN1, LOW); }
            // === PAGE HTML ===
            client.println("<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><link rel=\"icon\" href=\"data:,\">");
            client.println("<style>body { text-align: center; font-family: 'Trebuchet MS', Arial; margin-left:auto; margin-right:auto;} .slider { width: 300px; } .button { background-color: #195B6A; border: none; color: white; padding: 16px 40px; font-size: 30px; margin: 2px; cursor: pointer;} .button2 {background-color: #77878A;}</style>");
            client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");
            client.println("</head><body><h1>Auto pilot</h1>");
            client.println("<p>Direction: <span id=\"servoPos\"></span></p>");
            client.println("<input type=\"range\" min=\"0\" max=\"360\" class=\"slider\" id=\"servoSlider\" onchange=\"servo(this.value)\" value=\""+valueString+"\"/>");
            client.println("<p>Motor: " + INState + "</p>");
            if (INState=="stopped") client.println("<p><a href=\"/IN/running\"><button class=\"button \">RUN</button></a></p>");
            else client.println("<p><a href=\"/IN/stopped\"><button class=\"button button2\">STOP</button></a></p>");
            client.println("<script>var slider = document.getElementById(\"servoSlider\"); var servoP = document.getElementById(\"servoPos\"); servoP.innerHTML = slider.value; slider.oninput = function() { slider.value = this.value; servoP.innerHTML = this.value; } $.ajaxSetup({timeout:1000}); function servo(pos) { $.get(\"/?value=\" + pos + "&"); {Connection: close};}</script>");
            client.println("</body></html>");
            // Slider -> direction
            if(header.indexOf("GET /?value=")>=0) {
              pos1 = header.indexOf('='); pos2 = header.indexOf('&'); valueString = header.substring(pos1+1, pos2);
            }
            client.println(); break;
          } else { currentLine = ""; }
        } else if (c != '\r') { currentLine += c; }
      }
    }
    header = ""; client.stop(); Serial.println("Client déconnecté.\n");
  }
} 