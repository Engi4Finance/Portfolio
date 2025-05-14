# Main Robotique Sensitive à 5 Doigts

Ce dossier regroupe les codes principaux du projet de main robotique sensitive à 5 doigts, contrôlée par un gant équipé de capteurs de flexion et d'un accéléromètre.

## Description du projet

- **Main robotique à 5 doigts** : chaque doigt est actionné par un petit moteur DC.
- **Commande par gant** :
  - Les **capteurs de flexion** sur le gant permettent de contrôler individuellement les doigts de la main robotique (fermeture/ouverture proportionnelle à la flexion du doigt sur le gant).
  - Un **accéléromètre** placé sur le gant permet de contrôler le poignet de la main robotique sur deux axes (inclinaison, rotation).
- **Deux ESP32** :
  - Un **ESP32 côté gant** : lit les capteurs de flexion et l'accéléromètre, puis transmet les données sans fil.
  - Un **ESP32 côté main** : reçoit les données et pilote les moteurs de la main robotique (doigts et poignet).

## Organisation des codes

- `code_final_flex_moteur.ino` : contrôle 3 doigts de la main robotique à partir de 3 capteurs de flexion.
- `code_2_capteur_fx_2M.ino` : contrôle 2 doigts de la main robotique à partir de 2 capteurs de flexion.
- `code_final_accelerometre.ino` : contrôle le poignet (2 axes) de la main robotique à partir d'un accéléromètre MPU6050.

## Utilisation

1. **Téléverser** les codes sur les deux ESP32 :
   - Un ESP32 pour le gant (lecture des capteurs et transmission).
   - Un ESP32 pour la main robotique (réception et commande des moteurs).
2. **Connecter** les capteurs de flexion sur le gant aux entrées analogiques prévues.
3. **Connecter** l'accéléromètre MPU6050 sur le gant via I2C.
4. **Brancher** les moteurs DC de la main robotique sur les sorties prévues.
5. **Mettre sous tension** et tester le contrôle des doigts et du poignet via le gant.

## Fichiers présents
- `code_final_flex_moteur.ino`
- `code_2_capteur_fx_2M.ino`
- `code_final_accelerometre.ino`

---

**Auteur :** [Lionel FRITZ]

**Résumé :** Ce projet permet de piloter une main robotique à 5 doigts de façon intuitive et sensible grâce à un gant instrumenté et deux ESP32 communicants, pour des applications en robotique, biomécatronique ou prototypage avancé. 