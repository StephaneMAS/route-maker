# Mini Automated Surveillance Station
**PÔLE ROS — FIERI | Compétition Hebdomadaire — Semaine 2**  
Du 5 mai au 9 mai 2026

---

##  Membres du binôme
| Nom | GitHub |
|-----|--------|
| [Consolé AGOSSOU ] | [consoleagossou] |
| [Stéphane AVOCEVOU ] | [StephaneMAS] |

---

## 📋 Description du fonctionnement

Ce projet est une **mini station de surveillance automatisée** construite sur Arduino Uno. Le système surveille en continu deux capteurs (lumière et vibrations) et réagit automatiquement selon les conditions détectées.

### Les trois états du système

| État | Condition | LED RGB | Buzzer | Servo | Ventilateur |
|------|-----------|---------|--------|-------|-------------|
| **Veille (Normal)** | Lumière ≥ seuil, pas de vibration |  Verte | Silence | 0° | Arrêté |
| **Alerte** | Lumière < seuil OU vibration détectée |  Rouge | Alarme 1kHz | 90° | Actif |
| **Désarmé** | Commande IR reçue | Bleue | Silence | 0° | Arrêté |

### Logique de fonctionnement

1. Le **capteur LDR** (A0) mesure l'intensité lumineuse en continu.
2. Le **potentiomètre** (A1) permet de régler manuellement le seuil de déclenchement.
3. Si la lumière tombe **sous le seuil** → état ALERTE déclenché.
4. Si une **vibration** est détectée (capteur pin 2) → état ALERTE déclenché également.
5. Quand les conditions redeviennent normales → le système se **réinitialise automatiquement**.
6. Une **télécommande IR** permet d'armer ou désarmer le système à distance.

---

##  Liste du matériel

| Composant | Quantité | Broche Arduino |
|-----------|----------|---------------|
| Arduino Uno | 1 | — |
| LED RGB (cathode commune) | 1 | R→9, G→10, B→11 |
| Résistances 220 Ω | 3 | (en série avec LED RGB) |
| Buzzer piézoélectrique | 1 | 8 |
| LDR | 1 | A0 |
| Résistance 10 kΩ | 1 | (diviseur de tension LDR) |
| Potentiomètre 10 kΩ | 1 | A1 |
| Servomoteur SG90 | 1 | 6 |
| Capteur de vibration | 1 | 2 (interrupt) |
| Transistor NPN (2N2222) | 1 | — |
| Moteur DC (ventilateur) | 1 | 5 |
| Récepteur IR (TSOP4838) | 1 | 7 |
| Télécommande IR | 1 | — |
| Breadboard | 1 | — |
| Câbles de connexion | ~25 | — |
| Câble USB | 1 | — |

---

##  Composants substitués



> Exemple :  
> **Servomoteur SG90** remplacé par **servomoteur MG90S** — moteur équivalent avec
> le même protocole de contrôle PWM et la même plage angulaire (0–180°).

---

##  Bibliothèques utilisées

| Bibliothèque | Usage | Installation |
|---|---|---|
| `Servo.h` | Contrôle du servomoteur | Incluse par défaut dans Arduino IDE |
| `IRremote` | Réception des signaux IR | Arduino IDE → Gestionnaire de bibliothèques → "IRremote" by shirriff |

---

##  Comment trouver les codes IR de votre télécommande

Avant d'utiliser le Bonus 3, vous devez identifier les codes de votre télécommande :

1. Ouvrez Arduino IDE → **Fichier → Exemples → IRremote → IRrecvDump**
2. Changez la ligne `int RECV_PIN = 11;` en `int RECV_PIN = 7;`
3. Téléversez le sketch et ouvrez le **Moniteur Série** (9600 baud)
4. Appuyez sur les boutons de votre télécommande → notez les codes hexadécimaux affichés
5. Remplacez `IR_CODE_ARM` et `IR_CODE_DISARM` dans `mini_station.ino` avec vos codes

---

##  Difficultés rencontrées

- **Seuil LDR instable :** La valeur lue par le LDR varie légèrement même sans changement de lumière. Solution : utiliser le potentiomètre pour ajuster le seuil en temps réel, et ajouter un délai de 100ms entre les lectures.

- **Servo qui "tremble" :** Le servo recevait des commandes `write()` à chaque itération de loop(), causant des micro-mouvements. Solution : utiliser le flag `inAlert` pour n'envoyer la commande qu'une seule fois lors du changement d'état.

- **Codes IR non reconnus :** La valeur `0xFFFFFFFF` est souvent envoyée lors d'un appui long (répétition). Solution : ignorer cette valeur dans `handleIR()` ou ajouter une condition `if (code != 0xFFFFFFFF)`.



---

## Sources consultées

- [Arduino Reference — analogRead()](https://www.arduino.cc/reference/en/language/functions/analog-io/analogread/)
- [Arduino Reference — tone()](https://www.arduino.cc/reference/en/language/functions/advanced-io/tone/)
- [Arduino Reference — Servo library](https://www.arduino.cc/reference/en/libraries/servo/)
- [Arduino Built-in Example — Knob (servo + potentiomètre)](https://docs.arduino.cc/built-in-examples/servo/Knob/)
- [Arduino Built-in Example — Tone Melody](https://docs.arduino.cc/built-in-examples/digital/toneMelody/)
- [IRremote library documentation](https://github.com/Arduino-IRremote/Arduino-IRremote)
- Document DFRobot — Projets 3 à 15

---

##  Démonstration

> Voir le fichier `demo.mp4` dans ce dossier (ou lien ci-dessous).

La vidéo montre :
1. Le système en **état normal** (LED verte, servo à 0°, buzzer silencieux)
2. Le déclenchement de l'**alerte par obscurité** (couvrir le LDR avec la main)
3. La **réinitialisation automatique** quand la lumière revient
4. Le déclenchement de l'**alerte par vibration** (tapoter la breadboard)
5. Le **désarmement via télécommande IR** (LED bleue)
6. Le **réarmement via télécommande IR** (retour LED verte)

---


*A Défaut du temps ( les sortirs pedagogiques à l'EPAC et les programmes surchargés ) nous n'avions pas pu terminé se projet*

*Projet réalisé dans le cadre du PÔLE ROS — FIERI, Semaine 2, mai 2026.*

