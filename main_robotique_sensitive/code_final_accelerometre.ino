#include <Wire.h>
#include <MPU6050.h>

// Define the pins for the I2C bus
#define SDA1 21
#define SCL1 22

// Define the motor pins
const int motor1In1 = 2;          // Broche du moteur 1 connectée au pont en H de la pcb
const int motor1In2 = 0;          
const int motor1PWM = 4;          

const int motor2In1 = 18;         // Broche du moteur 2 connectée au pont en H de la pcb
const int motor2In2 = 19;         
const int motor2PWM = 5;        

MPU6050 mpu1(0x69);  // Notre accelerometre MPU6050 avec address 0x69

const int numReadings = 10; // contant de lectures de la moyenne glissante des valeurs de l'accelerometre

int readings1X[numReadings];
int readings1Y[numReadings];
int readings1Z[numReadings];

int readIndex = 0;
long total1X = 0, total1Y = 0, total1Z = 0;

int16_t prev_smooth1X = 0; // Stocker la valeur lisser de l'accelerometre sur l'axe X 
int16_t prev_smooth1Y = 0; // Stocker la valeur lisser de l'accelerometre sur l'axe Y

void setup() {
  Serial.begin(9600);

  // Initialiser Buse I2C
  Wire.begin(SDA1, SCL1);

  // Initialize notre accelerometre MPU6050
  mpu1.initialize();

  mpu1.setFullScaleAccelRange(MPU6050_ACCEL_FS_16); // adjustement de la sensibilité

  // Verification de la connection

  if (mpu1.testConnection()) {
    Serial.println("MPU6050 (0x69) trouvé sur le premier bus I2C");
  } else {
    Serial.println("Erreur de connexion avec MPU6050 (0x69) sur le premier bus I2C");
  }

  // configuration des pin moteurs en sortie
  pinMode(motor1In1, OUTPUT);
  pinMode(motor1In2, OUTPUT);
  pinMode(motor1PWM, OUTPUT);

  pinMode(motor2In1, OUTPUT);
  pinMode(motor2In2, OUTPUT);
  pinMode(motor2PWM, OUTPUT);

  // Initialiser les moteur en mode off
  digitalWrite(motor1In1, LOW);
  digitalWrite(motor1In2, LOW);
  analogWrite(motor1PWM, 0);

  digitalWrite(motor2In1, LOW);
  digitalWrite(motor2In2, LOW);
  analogWrite(motor2PWM, 0);

  // Initialiser les lecture de moyenne glissante à 0
  for (int i = 0; i < numReadings; i++) {
    readings1X[i] = 0;
    readings1Y[i] = 0;
    readings1Z[i] = 0;
  }
}

long smooth(int readings[], long &total, int newReading) {

  // Enlever la dernière lecture de la somme totale
  total = total - readings[readIndex];

  // Ajouter la nouvelle lecture
  readings[readIndex] = newReading;
  total = total + readings[readIndex];

  // Gérer l'indice de lecture pour rester dans les limites du tableau
  readIndex = (readIndex + 1) % numReadings;

  // Calculer la moyenne
  return total / numReadings;
}

void loop() {
  int16_t ax1, ay1, az1;

  // lecture des valeur de l'acceleration du capteur
  mpu1.getAcceleration(&ax1, &ay1, &az1);

  // Calcule de la valeur lisser en X,Y,Z
  long smooth1X = smooth(readings1X, total1X, ax1);
  long smooth1Y = smooth(readings1Y, total1Y, ay1);
  long smooth1Z = smooth(readings1Z, total1Z, az1);

  // Afficher les valeurs de l'acccelerometre lisser
  Serial.print(smooth1X);
  Serial.print("\t");
  Serial.print(smooth1Y);
  Serial.print("\t");
  Serial.print(smooth1Z);
  Serial.println();

  // Controle du motor1 basé sur la valeur de l'acceleromètre smooth1X
  if (abs(smooth1X) - abs(prev_smooth1X) > 40) {
    if (smooth1X < 0) {
      // Forward direction for motor1
      digitalWrite(motor1In1, LOW);
      digitalWrite(motor1In2, HIGH);
      analogWrite(motor1PWM, 255);
    } else if (smooth1X > 0) {
      // Reverse direction for motor1
      digitalWrite(motor1In1, HIGH);
      digitalWrite(motor1In2, LOW);
      analogWrite(motor1PWM, 255);
    }
  } else {
    // Arreter le moteur si il n'ya pas de variation significative
    digitalWrite(motor1In1, LOW);
    digitalWrite(motor1In2, LOW);
    analogWrite(motor1PWM, 0);
  }

  // Controle du motor2 basé sur la valeur de l'acceleromètre smooth1Y
  if (abs(smooth1Y) - abs(prev_smooth1Y) > 60) {
    if (smooth1Y > 0) {
      // Forward direction for motor2
      digitalWrite(motor2In1, HIGH);
      digitalWrite(motor2In2, LOW);
      analogWrite(motor2PWM, 255);
    } else if (smooth1Y < 0) {
      // Reverse direction for motor2
      digitalWrite(motor2In1, LOW);
      digitalWrite(motor2In2, HIGH);
      analogWrite(motor2PWM, 255);
    }
  } else {
    // Arreter le moteur si il n'ya pas de variation significative
    digitalWrite(motor2In1, LOW);
    digitalWrite(motor2In2, LOW);
    analogWrite(motor2PWM, 0);
  }

  // Meettre à jour les valeur precedente lissé
  prev_smooth1X = smooth1X;
  prev_smooth1Y = smooth1Y;

  delay(50);
} 