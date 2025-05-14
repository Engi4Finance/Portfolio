#include <WiFi.h> //include wifi library
#include <Servo.h> //include servo library for esp32

//include libraries necessary for the magnetometer
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
 
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345); //assigns a unique ID to the magnetometer

Servo myservo; //creates a new servo object

static const int servoPin = 13; //defines servo pin

float gap = 0; //defines gap
int servoposition = 90; //defines servo motor angle position

const int IN1 = 26; //defines IN1 pin of the h-bridge

//network credentials
const char* ssid = "POCOX3Pro";
const char* password = "Fribri58";

WiFiServer server(80); //set web server port number to 80

String header; //variable to store the HTTP request

//auxiliar variables to store the current output state
String valueString;
String INState = "stopped";

int pos1 = 0;
int pos2 = 0;

unsigned long currentTime = millis(); //current time
unsigned long previousTime = 0; //previous time
const long timeoutTime = 2000; //define timeout time in milliseconds

void setup() {
  Serial.begin(115200); //starts the serial monitor

  myservo.attach(servoPin);  //attach the servo to the previously defined pin

  //initialize the output variable as output and set the output to low
  pinMode(IN1, OUTPUT);
  digitalWrite(IN1, LOW);
 
  if(!mag.begin()) //test if the magnetometer is well connected
{
Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
while(1);
}
  //connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available(); //listen for incoming clients

  //get a new sensor event
  sensors_event_t event;
  mag.getEvent(&event);

  //hold the module so that Z is pointing 'up' and you can measure the heading with x&y
  //calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2(event.magnetic.y, event.magnetic.x);

  float declinationAngle = 1.22; //define declination angle of Toulouse (error of the magnetic field)
  heading += declinationAngle; //fix the magnetic field error

  //correct for when signs are reversed
  if(heading < 0)
  heading += 2*PI;

  //check for wrap due to addition of declination
  if(heading > 2*PI)
  heading -= 2*PI;

  //convert radians to degrees for readability
  float headingDegrees = heading * 180/M_PI;

  //print values for the magnetometer
  Serial.print("mag: ");
  Serial.println(headingDegrees);

  gap = valueString.toInt() - headingDegrees; //The gap is equal to the desired value minus the angle of the magnetometer

  //prints the desired direction value from the webpage slider
  Serial.print("slider: ");
  Serial.println(valueString.toInt());

  //adjust the gap to be between -180 and 180
  
  if (gap > 180) {
    gap = gap - 360;
  }

  if (gap < -180) {
    gap = gap + 360;
  }

  //will rotate the servo motor accordingly to the gap
  
  if (gap <= 2 && gap >= -2) {
    servoposition = 90;
  }

  else if (gap > 2 && gap <= 20) {
    servoposition = 95;
  }

  else if (gap > 20 && gap <= 40) {
    servoposition = 100;
  }

  else if (gap > 40 && gap <= 60) {
    servoposition = 105;
  }

  else if (gap > 60 && gap <= 80) {
    servoposition = 110;
  }

  else if (gap > 80 && gap <= 100) {
    servoposition = 115;
  }

  else if (gap > 100 && gap <= 120) {
    servoposition = 120;
  }

  else if (gap > 120 && gap <= 140) {
    servoposition = 125;
  }

  else if (gap > 140 && gap <= 160) {
    servoposition = 130;
  }

  else if (gap > 160 && gap <= 180) {
    servoposition = 135;
  }
 
  else if (gap < -2 && gap >= -20) {
    servoposition = 85;
  }

  else if (gap < -20 && gap >= -40) {
    servoposition = 80;
  }

  else if (gap < -40 && gap >= -60) {
    servoposition = 75;
  }

  else if (gap < -60 && gap >= -80) {
    servoposition = 70;
  }

  else if (gap < -80 && gap >= -100) {
    servoposition = 65;
  }

  else if (gap < -100 && gap >= -120) {
    servoposition = 60;
  }

  else if (gap < -120 && gap >= -140) {
    servoposition = 55;
  }

  else if (gap < -140 && gap >= -160) {
    servoposition = 50;
  }

  else {
    servoposition = 45;
  }

  myservo.write(servoposition); //the servo goes to the desired position

  if (client) { //if a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");  //print a message out in the serial port
    String currentLine = "";  //make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) { //loop while the client's connected
      currentTime = millis();
      currentTime = millis();
      if (client.available()) { //if there's bytes to read from the client,
        char c = client.read(); //read a byte, then
        Serial.write(c); //print it out the serial monitor
        header += c;
        if (c == '\n') { //if the byte is a newline character
          //if the current line is blank, you got two newline characters in a row.
          //that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            //HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            //and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

             //turns the pin on and off for the motor
            if (header.indexOf("GET /IN/run") >= 0) {
              INState = "running";
              digitalWrite(IN1, HIGH);
            } else if (header.indexOf("GET /IN/stop") >= 0) {
              INState = "stopped";
              digitalWrite(IN1, LOW);
            }

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");

            //CSS to style the webpage
            client.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial; margin-left:auto; margin-right:auto;}");
            client.println(".slider { width: 300px; }");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style>");
            client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");
                     
            //Web Page
            client.println("</head><body><h1>Auto pilot</h1>");
            client.println("<p>Direction: <span id=\"servoPos\"></span></p>");          
            client.println("<input type=\"range\" min=\"0\" max=\"360\" class=\"slider\" id=\"servoSlider\" onchange=\"servo(this.value)\" value=\""+valueString+"\"/>");
            
            //display current state, and RUN/STOPPED buttons for motor pin  
            //depending on the state it will show a different icon, button state and button
            client.println("<p>Motor: " + INState + "</p>");
            if (INState=="stopped") {
              client.println("<p><a href=\"/IN/running\"><button class=\"button \">RUN</button></a></p>");
            } else {
              client.println("<p><a href=\"/IN/stopped\"><button class=\"button button2\">STOP</button></a></p>");
            }
           
            //javascript to get the values for the direction slider
            client.println("<script>var slider = document.getElementById(\"servoSlider\");");
            client.println("var servoP = document.getElementById(\"servoPos\"); servoP.innerHTML = slider.value;");
            client.println("slider.oninput = function() { slider.value = this.value; servoP.innerHTML = this.value; }");
            client.println("$.ajaxSetup({timeout:1000}); function servo(pos) { ");
            client.println("$.get(\"/?value=\" + pos + "&"); {Connection: close};}</script>");
           
            client.println("</body></html>");    
           
            //gets value of the slider
            if(header.indexOf("GET /?value=")>=0) {
              pos1 = header.indexOf('=');
              pos2 = header.indexOf('&');
             
              valueString = header.substring(pos1+1, pos2);
             
             
            }        
            
            client.println(); //the HTTP response ends with another blank line
            
            break; //break out of the while loop
          } else { //if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  //if you got anything else but a carriage return character,
          currentLine += c; //add it to the end of the currentLine
        }
      }
    }
    
    header = ""; //clear the header variable
    
    client.stop(); //close the connection
    Serial.println("Client disconnected.");
    Serial.println("");
  }
} 