# ESP32 Soil NPK Sensor (RS485/Modbus)

Ce projet permet de lire les valeurs d'azote (N), phosphore (P), potassium (K) d'un capteur NPK via RS485/Modbus avec un ESP32.

## Fichier
- `npk_sensor.ino` : Code Arduino pour lire les valeurs NPK et les afficher sur le port série.

## Dépendances
- ESP32 (avec port Serial2)
- [HardwareSerial.h](https://www.arduino.cc/reference/en/language/functions/communication/serial/hardwareserial/)
- Module RS485 (ex : MAX485)
- Capteur NPK compatible Modbus

## Branchement
- **RS485 -> ESP32**
  - RE : GPIO 14
  - DE : GPIO 13
  - RO (RS485) -> RX2 (GPIO 16)
  - DI (RS485) -> TX2 (GPIO 17)
- Alimente le capteur et le module RS485 selon leur documentation.

## Utilisation
1. Branche le matériel comme indiqué ci-dessus.
2. Téléverse le code sur l'ESP32 via l'IDE Arduino.
3. Ouvre le moniteur série (baudrate 115200).
4. Les valeurs NPK s'affichent toutes les secondes.

## Protocole
- Le code envoie des trames Modbus pour interroger chaque valeur (N, P, K).
- Les réponses sont lues et affichées en mg/kg.

## Conseils
- Vérifie l'adresse et la documentation de ton capteur NPK (certaines trames peuvent varier).
- Utilise une alimentation stable pour le capteur et le module RS485.
- Pour des mesures fiables, évite les interférences sur les lignes RS485.

## Auteur
Lionel FRITZ 