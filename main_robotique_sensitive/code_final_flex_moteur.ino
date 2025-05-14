const int flexPin1 = 34;          
const int flexPin2 = 13;
const int flexPin3 = 15; 

const int motor1In1 = 18;         // moteur 1
const int motor1In2 = 5;          
const int motor1PWM = 19;

const int motor2In1 = 32;         //moteur 2
const int motor2In2 = 35;        
const int motor2PWM = 33;  


const int motor3In1 = 27;         //moteur 3
const int motor3In2 = 14;        
const int motor3PWM = 26; 

const int minFlexChange = 15;  



int prevFlexValue1;               // Stockage de la valeur précédente du premier capteur de flexion
int prevFlexValue2;               // Stockage de la valeur précédente du deuxième capteur de flexion
int prevFlexValue3;
bool motor1Active;                // Indicateur indiquant si le premier moteur est actif ou non
bool motor2Active;                // Indicateur indiquant si le deuxième moteur est actif ou non
bool motor3Active;



void setup() {

  Serial.begin(9600);             // Initialisation de la communication série
  pinMode(motor1In1, OUTPUT);     // Définir les broches de direction du premier moteur en sortie
  pinMode(motor1In2, OUTPUT);
  pinMode(motor1PWM, OUTPUT);     // Définir la broche PWM comme sortie pour le premier moteur

  pinMode(motor2In1, OUTPUT);     // Définir les broches de direction du deuxième moteur en sortie
  pinMode(motor2In2, OUTPUT);
  pinMode(motor2PWM, OUTPUT);     // Définir la broche PWM comme sortie pour le deuxième moteur

  pinMode(motor3In1, OUTPUT);     // Définir les broches de direction du troisième moteur en sortie
  pinMode(motor3In2, OUTPUT);
  pinMode(motor3PWM, OUTPUT);

  motor1Active = false;           // Initialiser l'état du premier moteur comme inactif
  motor2Active = false;           // Initialiser l'état du deuxième moteur comme inactif
  motor3Active = false;
  

  // Initialiser la direction des moteurs (si nécessaire)
  digitalWrite(motor1In1, LOW);
  digitalWrite(motor1In2, LOW);
  digitalWrite(motor2In1, LOW);
  digitalWrite(motor2In2, LOW);
  digitalWrite(motor3In1, LOW);
  digitalWrite(motor3In2, LOW);

  // Augmenter la résolution ADC à 13 bits (0-8191)
  analogReadResolution(8);
 

  // Lecture initiale des capteurs de flexion
  prevFlexValue1 = analogRead(flexPin1);
  prevFlexValue2 = analogRead(flexPin2);
  prevFlexValue3 = analogRead(flexPin3);
}

void loop() {
  int flexValue1 = analogRead(flexPin1); // Lecture de la valeur du premier capteur de flexion
  int flexValue2 = analogRead(flexPin2); // Lecture de la valeur du deuxième capteur de flexion
  int flexValue3 = analogRead(flexPin3);


  // Mapper les valeurs des capteurs de flexion à une plage de 0 à 255
  int pwmValue1 = map(flexValue1, 115, 55, 0, 255);//ref 422 09 sur le doigt ANNULAIRE plage de 115 min à 55 max pli

  int pwmValue2 = map(flexValue2, 100, 30, 0, 255);//ref 0222 12 sur le doigt Majeur plage de 140 min à 50 max plier

  int pwmValue3 = map(flexValue3, 123, 30, 0, 255);//ref 0422 27 sur le doigt INDEX plage de 80 min à 30 max plier

  // int pwmValue2 = map(flexValue2, 140, 50, 0, 255);//ref 0422 14 sur le doigt Majeur plage de 140 min à 50 max plier

  // Contrôle du premier moteur 
  if (abs(flexValue1 - prevFlexValue1) >= minFlexChange) {
    
    if (flexValue1 < prevFlexValue1) {
      // Sens de rotation positif pour le premier moteur
      digitalWrite(motor1In1, HIGH);
      digitalWrite(motor1In2, LOW);
    } else {
      // Sens de rotation négatif pour le premier moteur
      digitalWrite(motor1In1, LOW);
      digitalWrite(motor1In2, HIGH);
    }

    // Écrire la valeur PWM sur la broche PWM du premier moteur
    analogWrite(motor1PWM, pwmValue1);

    // Indiquer que le premier moteur est actif
    motor1Active = true;

    // Afficher la valeur du premier capteur de flexion et la valeur PWM pour le débogage
   /* Serial.print("Flex Value 1: ");
    Serial.println(flexValue1);
    Serial.print(" - PWM Value 1: ");
    Serial.println(pwmValue1);*/

    // Mettre à jour la valeur précédente du premier capteur de flexion
    prevFlexValue1 = flexValue1;
  } else if (motor1Active) {
    // Arrêter le premier moteur
    digitalWrite(motor1In1, LOW);
    digitalWrite(motor1In2, LOW);
    analogWrite(motor1PWM, 0);
    motor1Active = false; // Indiquer que le premier moteur est inactif
  }

  // Contrôle du deuxième moteur
  if (abs(flexValue2 - prevFlexValue2) >= minFlexChange) {
    if (flexValue2 < prevFlexValue2) {
      // Sens de rotation positif pour le deuxième moteur
      digitalWrite(motor2In1, HIGH);
      digitalWrite(motor2In2, LOW);
    } else {
      // Sens de rotation négatif pour le deuxième moteur
      digitalWrite(motor2In1, LOW);
      digitalWrite(motor2In2, HIGH);
    }

    // Écrire la valeur PWM sur la broche PWM du deuxième moteur
    analogWrite(motor2PWM, pwmValue2);

    // Indiquer que le deuxième moteur est actif
    motor2Active = true;

    // Afficher la valeur du deuxième capteur de flexion et la valeur PWM pour le débogage
    /*Serial.print("Flex Value 2: ");
    Serial.print(flexValue2);
    Serial.print(" - PWM Value 2: ");
    Serial.println(pwmValue2);*/

    // Mettre à jour la valeur précédente du deuxième capteur de flexion
    prevFlexValue2 = flexValue2;
  } else if (motor2Active) {
    // Arrêter le deuxième moteur
    digitalWrite(motor2In1, LOW);
    digitalWrite(motor2In2, LOW);
    analogWrite(motor2PWM, 0);
    motor2Active = false; // Indiquer que le deuxième moteur est inactif
  }


  // Contrôle du troisieme moteur
  if (abs(flexValue3 - prevFlexValue3) >= minFlexChange) {
    if (flexValue3 < prevFlexValue3) {
      // Sens de rotation positif pour le deuxième moteur
      digitalWrite(motor3In1, HIGH);
      digitalWrite(motor3In2, LOW);
    } else {
      // Sens de rotation négatif pour le deuxième moteur
      digitalWrite(motor3In1, LOW);
      digitalWrite(motor3In2, HIGH);
    }

    // Écrire la valeur PWM sur la broche PWM du deuxième moteur
    analogWrite(motor3PWM, pwmValue3);

    // Indiquer que le troisième moteur est actif
    motor3Active = true;

    // Afficher la valeur du troisieme capteur de flexion et la valeur PWM pour le débogage
    Serial.print("Flex Value 3: ");
    Serial.print(flexValue3);
    Serial.print(" - PWM Value 3: ");
    Serial.println(pwmValue3);

    // Mettre à jour la valeur précédente du troisieme capteur de flexion
    prevFlexValue3 = flexValue3;
  }
   else if (motor3Active) {
    // Arrêter le deuxième moteur
    digitalWrite(motor3In1, LOW);
    digitalWrite(motor3In2, LOW);
    analogWrite(motor3PWM, 0);
    motor3Active = false; // Indiquer que le troisième moteur est inactif
  }

  delay(500); // Délai entre chaque lecture
} 