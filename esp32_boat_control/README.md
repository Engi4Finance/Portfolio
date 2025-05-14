# ESP32 Boat Control & Autopilot

Ce projet regroupe deux programmes pour ESP32 permettant de contrôler un bateau à distance via WiFi :
- **Boat Control** : Contrôle manuel du bateau (relais, moteur, servo) via une interface web.
- **Autopilot** : Contrôle automatique de la direction à l'aide d'un capteur magnétomètre (boussole) et d'une interface web.

## Fichiers
- `boat_control.ino` : Contrôle manuel (relais, moteur, servo) via WiFi.
- `autopilot.ino` : Autopilot avec capteur magnétomètre (Adafruit HMC5883).

## Dépendances Arduino
- [WiFi.h](https://www.arduino.cc/en/Reference/WiFi)
- [Servo.h](https://www.arduino.cc/en/Reference/Servo)
- [Adafruit HMC5883_U](https://github.com/adafruit/Adafruit_HMC5883_Unified) (pour autopilot)
- [Adafruit Sensor](https://github.com/adafruit/Adafruit_Sensor) (pour autopilot)

Installe ces librairies via le gestionnaire de bibliothèques de l'IDE Arduino.

## Utilisation
1. **Configure les paramètres WiFi** en haut des fichiers `.ino` (`ssid`, `password`).
2. **Branche le matériel** :
   - Servo sur la pin 13
   - Relais sur la pin 17
   - H-bridge (moteur) sur la pin 26
   - (Autopilot) Capteur HMC5883 sur I2C (SDA/SCL)
3. **Téléverse le code** sur ton ESP32 via l'IDE Arduino.
4. **Connecte-toi au même réseau WiFi** que l'ESP32.
5. **Ouvre l'adresse IP affichée dans le moniteur série** dans ton navigateur pour accéder à l'interface web.

## Sécurité
- **Ne publie jamais tes identifiants WiFi en clair** dans un code public !
- Pour un usage réel, utilise un réseau dédié ou ajoute une authentification.

## Auteur
Lionel FRITZ

---

**N'hésite pas à adapter le code selon tes besoins et à proposer des améliorations !** 