/*
 * ESP32 Boat Control
 * Contrôle d'un bateau via WiFi (serveur web embarqué)
 * Permet de piloter un relais (canon à eau), un moteur (H-bridge) et un servo moteur.
 *
 * Auteur : Lionel FRITZ
 * Dernière mise à jour : 2024
 *
 * Dépendances Arduino :
 *   - WiFi.h
 *   - Servo.h
 *
 * Sécurité :
 *   - Ne pas exposer ce code en production avec des identifiants WiFi en clair !
 */

// === CONFIGURATION ===
#include <WiFi.h>
#include <Servo.h>

const char* ssid = "POCOX3Pro";      // SSID WiFi
const char* password = "Fribri58";   // Mot de passe WiFi

const int RelayPin =  17;             // Pin du relais (canon à eau)
const int IN1 = 26;                   // Pin du H-bridge (moteur)
const int servoPin = 13;              // Pin du servo moteur

// === VARIABLES GLOBALES ===
Servo myservo;
byte relayMode = 0;
byte crm = 0;
WiFiServer server(80);
String header;
String output5State = "off";
String INState = "stopped";
String valueString;
int pos1 = 0, pos2 = 0;
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

// === SETUP ===
void setup() {
  Serial.begin(115200);
  pinMode(RelayPin, OUTPUT); digitalWrite(RelayPin, LOW);
  pinMode(IN1, OUTPUT); digitalWrite(IN1, LOW);
  myservo.attach(servoPin);

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
void loop() {
  WiFiClient client = server.available();
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
            // === TRAITEMENT DES REQUÊTES ===
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            // Relais
            if (header.indexOf("GET /5/on") >= 0) { output5State = "on"; digitalWrite(RelayPin, HIGH); }
            else if (header.indexOf("GET /5/off") >= 0) { output5State = "off"; digitalWrite(RelayPin, LOW); }
            // Moteur
            if (header.indexOf("GET /IN/run") >= 0) { INState = "running"; digitalWrite(IN1, HIGH); }
            else if (header.indexOf("GET /IN/stop") >= 0) { INState = "stopped"; digitalWrite(IN1, LOW); }
            // === PAGE HTML ===
            client.println("<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { background-color: #203345; font-family: Courier; text-align: center; color: white } p { font-size: 25px; } h1 { font-size: 40px; font-weight:bold; } .slider { width: 300px; accent-color:#195B6A; } .button { background-color: #195B6A; border: none; color: white; padding: 16px 40px; font-size: 30px; margin: 2px; cursor: pointer;} .button2 {background-color: #77878A;}</style>");
            client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script><link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css\">");
            client.println("</head><body><h1>Boat Control</h1>");
            client.println("<p>Position: <span id=\"servoPos\"></span> <i style=\"font-size:24px; margin-left: 5px;\" class=\"fa\">&#xf124;</i></p>");
            client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider\" id=\"servoSlider\" onchange=\"servo(this.value)\" value=\""+valueString+"\"/>");
            client.println("<p>Water canon: " + output5State + "</p>");
            if (output5State=="off") client.println("<p><i style=\"font-size:24px; margin-right: 5px;\" class=\"fa\">&#xf00d;</i><a href=\"/5/on\"><button class=\"button \">ON</button></a></p>");
            else client.println("<p><i style=\"font-size:24px; margin-right: 5px;\" class=\"fa\">&#xf043;</i><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>");
            client.println("<p>Motor: " + INState + "</p>");
            if (INState=="stopped") client.println("<p><i style=\"font-size:24px; margin-right: 5px;\" class=\"fa\">&#xf05e;</i><a href=\"/IN/running\"><button class=\"button \">RUN</button></a></p>");
            else client.println("<p><i style=\"font-size:24px; margin-right: 5px;\" class=\"fa\">&#xf0e4;</i><a href=\"/IN/stopped\"><button class=\"button button2\">STOP</button></a></p>");
            client.println("<i class=\"fa fa-anchor\" style=\"font-size:160px; margin:30px\"></i>");
            client.println("<script>var slider = document.getElementById(\"servoSlider\"); var servoP = document.getElementById(\"servoPos\"); servoP.innerHTML = slider.value; slider.oninput = function() { slider.value = this.value; servoP.innerHTML = this.value; } $.ajaxSetup({timeout:1000}); function servo(pos) { $.get(\"/?value=\" + pos + "&"); {Connection: close};}</script>");
            client.println("</body></html>");
            // Slider -> servo
            if(header.indexOf("GET /?value=")>=0) {
              pos1 = header.indexOf('='); pos2 = header.indexOf('&'); valueString = header.substring(pos1+1, pos2);
              myservo.write(valueString.toInt()); Serial.println(valueString);
            }
            client.println(); break;
          } else { currentLine = ""; }
        } else if (c != '\r') { currentLine += c; }
      }
    }
    header = ""; client.stop(); Serial.println("Client déconnecté.\n");
  }
} 