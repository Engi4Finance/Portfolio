#include <WiFi.h> //include wifi library
#include <Servo.h> //include servo library for esp32

//network credentials
const char* ssid = "POCOX3Pro";
const char* password = "Fribri58";

const int RelayPin =  17; //defines the pin of the relay
const int IN1 = 26; //defines IN1 pin of the h-bridge

Servo myservo; //creates a new servo object
static const int servoPin = 13; //defines servo pin

byte crm; //holds if command was recieved to change relay mode
byte  relayMode; //holds current relay state

WiFiServer server(80); //set web server port number to 80

String header; //variable to store the HTTP request

//auxiliar variables to store the current output state
String output5State = "off";
String INState = "stopped";
String valueString;

int pos1 = 0;
int pos2 = 0;

unsigned long currentTime = millis(); //current time
unsigned long previousTime = 0; //previous time
const long timeoutTime = 2000; //define timeout time in milliseconds

void setup() {
  Serial.begin(115200); //starts the serial monitor
  
  //initialize the output variables as outputs and set outputs to low
  
  pinMode(RelayPin, OUTPUT);
  digitalWrite(RelayPin, LOW);

  pinMode(IN1, OUTPUT);
  digitalWrite(IN1, LOW);

  myservo.attach(servoPin); //attach the servo to the previously defined pin

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
  relayMode = 0;
  crm = 0;
}

void loop() {
  WiFiClient client = server.available(); //listen for incoming clients

  if (client) { //if a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client."); //print a message out in the serial port
    String currentLine = ""; //make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) { //loop while the client's connected
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

            //turns the pin on and off for the relay
            if (header.indexOf("GET /5/on") >= 0) {
              Serial.println("Relay on");
              output5State = "on";
              digitalWrite(RelayPin, HIGH);
            } else if (header.indexOf("GET /5/off") >= 0) {
              Serial.println("Relay off");
              output5State = "off";
              digitalWrite(RelayPin, LOW);
            }

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
            client.println("<style>html { background-color: #203345; font-family: Courier; display: inline-block; margin: 0px auto; text-align: center; color: white }" );
            client.println("p { font-size: 25px; }");
            client.println("h1 { font-size: 40px; font-weight:bold; }");
            client.println(".slider { width: 300px; accent-color:#195B6A; }");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style>");
            client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");
            client.println("<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css\">");
           
            //Web Page
            client.println("</head><body><h1>Boat Control</h1>");
            client.println("<p>Position: <span id=\"servoPos\"></span> <i style=\"font-size:24px; margin-left: 5px;\" class=\"fa\">&#xf124;</i></p>");          
            client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider\" id=\"servoSlider\" onchange=\"servo(this.value)\" value=\""+valueString+"\"/>");
           
            //display current state, and ON/OFF buttons for relay pin  
            //depending on the state it will show a different icon, button state and button
            client.println("<p>Water canon: " + output5State + "</p>");    
            if (output5State=="off") {
              client.println("<p><i style=\"font-size:24px; margin-right: 5px;\" class=\"fa\">&#xf00d;</i><a href=\"/5/on\"><button class=\"button \">ON</button></a></p>");
            } else {
              client.println("<p><i style=\"font-size:24px; margin-right: 5px;\" class=\"fa\">&#xf043;</i><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            //display current state, and RUN/STOPPED buttons for motor pin  
            //depending on the state it will show a different icon, button state and button
            client.println("<p>Motor: " + INState + "</p>");
            if (INState=="stopped") {
              client.println("<p><i style=\"font-size:24px; margin-right: 5px;\" class=\"fa\">&#xf05e;</i><a href=\"/IN/running\"><button class=\"button \">RUN</button></a></p>");
            } else {
              client.println("<p><i style=\"font-size:24px; margin-right: 5px;\" class=\"fa\">&#xf0e4;</i><a href=\"/IN/stopped\"><button class=\"button button2\">STOP</button></a></p>");
            }

            client.println("<i class=\"fa fa-anchor\" style=\"font-size:160px; margin:30px\"></i>");

            //javascript to get the values for the servo slider
            client.println("<script>var slider = document.getElementById(\"servoSlider\");");
            client.println("var servoP = document.getElementById(\"servoPos\"); servoP.innerHTML = slider.value;");
            client.println("slider.oninput = function() { slider.value = this.value; servoP.innerHTML = this.value; }");
            client.println("$.ajaxSetup({timeout:1000}); function servo(pos) { ");
            client.println("$.get(\"/?value=\" + pos + "&"); {Connection: close};}</script>");
           
            client.println("</body></html>");

            //gets value of the slider and sends to the servo
            if(header.indexOf("GET /?value=")>=0) {
              pos1 = header.indexOf('=');
              pos2 = header.indexOf('&');
              valueString = header.substring(pos1+1, pos2);
             
              //Rotate the servo
              myservo.write(valueString.toInt());
              Serial.println(valueString);
            }
           
            client.println(); //the HTTP response ends with another blank line
            
            break; //break out of the while loop
          } else { //if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') { //if you got anything else but a carriage return character,
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