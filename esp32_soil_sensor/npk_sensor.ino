/*
 * ESP32 Soil NPK Sensor Reader (RS485/Modbus)
 *
 * Lit les valeurs d'azote (N), phosphore (P), potassium (K) d'un capteur NPK via RS485/Modbus
 * Affiche les résultats sur le port série.
 *
 * Auteur : Lionel FRITZ
 * Dernière mise à jour : 2024
 *
 * Dépendances :
 *   - Utilise Serial2 (RX=16, TX=17 par défaut sur ESP32)
 *   - Câblage RS485 : voir README
 */

#include <HardwareSerial.h>

#define RE 14   // Pin RE du module RS485
#define DE 13   // Pin DE du module RS485

// Commandes Modbus pour chaque capteur (voir doc du capteur)
const byte nitro[] = {0x01, 0x03, 0x00, 0x1e, 0x00, 0x01,0xB5 ,0xCC};
const byte phos[]  = {0x01, 0x03, 0x00, 0x1f, 0x00, 0x01,0xE4, 0x0C};
const byte pota[]  = {0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xC0};

byte values[7];

void setup() {
  // RX2 = GPIO16, TX2 = GPIO17 sur ESP32
  Serial2.begin(4800, SERIAL_8N1, 16, 17);
  Serial.begin(115200);
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);
  digitalWrite(RE, HIGH);   // Mode réception
  digitalWrite(DE, LOW);
  Serial.println("Setup complete. Starting loop...");
}

void loop() {
  byte val1, val2, val3;
  val1 = readSensor(nitro); delay(250);
  val2 = readSensor(phos);  delay(250);
  val3 = readSensor(pota);  delay(250);
  Serial.print("Nitrogen: "); Serial.print(val1); Serial.println(" mg/kg");
  Serial.print("Phosphorous: "); Serial.print(val2); Serial.println(" mg/kg");
  Serial.print("Potassium: "); Serial.print(val3); Serial.println(" mg/kg");
  delay(1000);
}

// Fonction de lecture d'un capteur NPK via Modbus
byte readSensor(const byte* command) {
  digitalWrite(DE, HIGH);   // Transmission
  digitalWrite(RE, HIGH);
  delay(10);
  Serial2.flush();    // Vide le buffer
  Serial.println("Envoi de la commande...");
  if (Serial2.write(command, 8) == 8) {
    Serial2.flush(); delay(10);
    digitalWrite(DE, LOW);   // Réception
    digitalWrite(RE, LOW);
    unsigned long startTime = millis();
    while (Serial2.available() < 8) {
      if (millis() - startTime >  1000) {
        Serial.println("Timeout de réponse");
        return 0;
      }
    }
    for (byte i = 0; i < 7; i++) {
      if (Serial2.available()) {
        values[i] = Serial2.read();
        Serial.print(values[i], HEX); Serial.print(" ");
      } else {
        Serial.println("Réponse incomplète");
        return 0;
      }
    }
    Serial.println();
    return values[3];   // Valeur utile dans le 4e octet
  } else {
    Serial.println("Echec d'envoi de la commande");
    return 0;
  }
} 