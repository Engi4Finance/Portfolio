const int flexPin1 = 34;          
const int flexPin2 = 12;
const int flexPin3 = 13; 

const int motor1In1 = 18;         
const int motor1In2 = 5;          
const int motor1PWM = 19;         
const int motor2In1 = 32;         
const int motor2In2 = 35;        
const int motor2PWM = 33;  
const int motor3In1 = 27;         
const int motor3In2 = 14;        
const int motor3PWM = 26; 

const int minFlexChange = 15;  

int prevFlexValue1;               // Stockage de la valeur précédente du premier capteur de flexion
int prevFlexValue2;               // Stockage de la valeur précédente du deuxième capteur de flexion
int prevFlexValue3;               // Stockage de la valeur précédente du troisième capteur de flexion

bool motor1Active;                // Indicateur indiquant si le premier moteur est actif ou non
bool motor2Active;                // Indicateur indiquant si le deuxième moteur est actif ou non
bool motor3Active;                // Indicateur indiquant si le troisième moteur est actif ou non


void setup() {
  Serial.begin(9600);             // Initialisation de la communication série
  
  // Définir les broches de direction et PWM des moteurs en sortie
  pinMode(motor1In1, OUTPUT);
  pinMode(motor1In2, OUTPUT);
  pinMode(motor1PWM, OUTPUT);

  pinMode(motor2In1, OUTPUT);
  pinMode(motor2In2, OUTPUT);
  pinMode(motor2PWM, OUTPUT);

  pinMode(motor3In1, OUTPUT);
  pinMode(motor3In2, OUTPUT);
  pinMode(motor3PWM, OUTPUT);

  motor1Active = false;           // Initialiser l'état des moteurs comme inactif
  motor2Active = false;
  motor3Active = false;

  // Initialiser la direction des moteurs (si nécessaire)
  digitalWrite(motor1In1, LOW);
  digitalWrite(motor1In2, LOW);
  digitalWrite(motor2In1, LOW);
  digitalWrite(motor2In2, LOW);
  digitalWrite(motor3In1, LOW);
  digitalWrite(motor3In2, LOW);

  // Configurer la résolution ADC
  analogReadResolution(8);

  // Lecture initiale des capteurs de flexion
  prevFlexValue1 = analogRead(flexPin1);
  prevFlexValue2 = analogRead(flexPin2);
  prevFlexValue3 = analogRead(flexPin3);
}

void loop() {
  int flexValue1 = analogRead(flexPin1); // Lecture de la valeur du premier capteur de flexion
  int flexValue2 = analogRead(flexPin2); // Lecture de la valeur du deuxième capteur de flexion
  int flexValue3 = analogRead(flexPin3); // Lecture de la valeur du troisième capteur de flexion

  // Mapper les valeurs des capteurs de flexion à une plage de 0 à 255
  int pwmValue1 = map(flexValue1, 115, 55, 0, 255); // Plage pour le capteur 1
  int pwmValue2 = map(flexValue2, 140, 50, 0, 255); // Plage pour le capteur 2
  int pwmValue3 = map(flexValue3, 80, 30, 0, 255);  // Plage pour le capteur 3

  // Contrôle du premier moteur
  if (abs(flexValue1 - prevFlexValue1) >= minFlexChange) {
    if (flexValue1 < prevFlexValue1) {
      digitalWrite(motor1In1, HIGH);
      digitalWrite(motor1In2, LOW);
    } else {
      digitalWrite(motor1In1, LOW);
      digitalWrite(motor1In2, HIGH);
    }
    analogWrite(motor1PWM, pwmValue1);
    motor1Active = true;
    Serial.print("Flex Value 1: ");
    Serial.print(flexValue1);
    Serial.print(" - PWM Value 1: ");
    Serial.println(pwmValue1);
    prevFlexValue1 = flexValue1;
  } else if (motor1Active) {
    digitalWrite(motor1In1, LOW);
    digitalWrite(motor1In2, LOW);
    analogWrite(motor1PWM, 0);
    motor1Active = false;
  }

  // Contrôle du deuxième moteur
  if (abs(flexValue2 - prevFlexValue2) >= minFlexChange) {
    if (flexValue2 < prevFlexValue2) {
      digitalWrite(motor2In1, HIGH);
      digitalWrite(motor2In2, LOW);
    } else {
      digitalWrite(motor2In1, LOW);
      digitalWrite(motor2In2, HIGH);
    }
    analogWrite(motor2PWM, pwmValue2);
    motor2Active = true;
    Serial.print("Flex Value 2: ");
    Serial.print(flexValue2);
    Serial.print(" - PWM Value 2: ");
    Serial.println(pwmValue2);
    prevFlexValue2 = flexValue2;
  } else if (motor2Active) {
    digitalWrite(motor2In1, LOW);
    digitalWrite(motor2In2, LOW);
    analogWrite(motor2PWM, 0);
    motor2Active = false;
  }

  // Contrôle du troisième moteur
  if (abs(flexValue3 - prevFlexValue3) >= minFlexChange) {
    if (flexValue3 < prevFlexValue3) {
      digitalWrite(motor3In1, HIGH);
      digitalWrite(motor3In2, LOW);
    } else {
      digitalWrite(motor3In1, LOW);
      digitalWrite(motor3In2, HIGH);
    }
    analogWrite(motor3PWM, pwmValue3);
    motor3Active = true;
    Serial.print("Flex Value 3: ");
    Serial.print(flexValue3);
    Serial.print(" - PWM Value 3: ");
    Serial.println(pwmValue3);
    prevFlexValue3 = flexValue3;

  } else if (motor3Active) {
    digitalWrite(motor3In1, LOW);
    digitalWrite(motor3In2, LOW);
    analogWrite(motor3PWM, 0);
    motor3Active = false;
  }

  delay(500); // Délai entre chaque lecture
} 