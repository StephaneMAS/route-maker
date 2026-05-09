# Mini Automated Surveillance Station
**PÔLE ROS — FIERI | Compétition Hebdomadaire — Semaine 2**  
Du 5 mai au 9 mai 2026

---

##  Membres du binôme

  | Rôle |                   Nom complet             | GitHub |        Étapes réalisées |
  |------|                   |--------|             ------------
| Membre 1 |            [Stephane AVOCEVOU ]     [ StephaneMAS ]   |      Étapes 1 → 4 |
| Membre 2 |              [Consolé AGOSSOU]       [console agossou]  |    Étapes 5 → 8 |

---

## 🔨 Répartition du travail

### Membre 1 — Fondations du système (Étapes 1 à 4)

**Étape 1 — Lecture du capteur LDR**  
Mise en place du circuit LDR avec diviseur de tension. Premier contact avec `analogRead()` et le Serial Monitor pour visualiser les valeurs brutes (0–1023) en fonction de la lumière ambiante.

**Étape 2 — LED RGB réactive**  
Câblage de la LED RGB sur les pins PWM 9, 10, 11. Implémentation de la première logique d'état : LED verte en veille, rouge en alerte, avec un seuil fixe défini dans le code.

**Étape 3 — Seuil ajustable avec le potentiomètre**  
Ajout du potentiomètre sur A1. Utilisation de `map()` pour convertir la valeur brute (0–1023) en un seuil dynamique (200–800), rendant la sensibilité du système réglable en temps réel sans modifier le code.

**Étape 4 — Alarme sonore avec le buzzer**  
Câblage du buzzer piézoélectrique sur la pin 8. Implémentation de `tone()` / `noTone()` pour déclencher une alarme à 1000 Hz lors du passage en état d'alerte.

---

### Membre 2 — Actionneurs, capteurs avancés et bonus (Étapes 5 à 8)

**Étape 5 — Servomoteur + gestion propre des états** *(projet obligatoire complet)*  
Intégration du servomoteur SG90 via la bibliothèque `Servo.h`. Ajout du flag `inAlert` pour éviter les appels répétés inutiles à `alertServo.write()` à chaque itération de la boucle. Le servo tourne à 90° en alerte et revient à 0° lors de la réinitialisation.

**Étape 6 — Capteur de vibration (Bonus 1)**  
Branchement du capteur de vibration sur la pin 2 (seule pin supportant les interruptions matérielles sur Arduino Uno). Utilisation d'une ISR (*Interrupt Service Routine*) avec `attachInterrupt()` pour détecter des vibrations même très courtes, indépendamment du `delay()` en cours dans la boucle principale.

**Étape 7 — Ventilateur via transistor NPN (Bonus 2)**  
Ajout du moteur DC contrôlé par un transistor NPN (2N2222). La pin 5 pilote la base du transistor pour commuter le circuit moteur, qui consomme bien plus que ce qu'une pin Arduino peut fournir directement. Le ventilateur s'active dès le déclenchement de l'alerte.

**Étape 8 — Télécommande IR : armement / désarmement (Bonus 3)**  
Intégration du récepteur IR (TSOP4838) sur la pin 7 avec la bibliothèque `IRremote`. Décodage des signaux 38 kHz en codes hexadécimaux. Deux boutons de la télécommande permettent d'armer (LED verte) ou désarmer (LED bleue) le système à distance. La valeur `0xFFFFFFFF` (code de répétition IR) est filtrée pour éviter les faux déclenchements.

---

##  Fonctionnement général du système

### Les trois états

| État | Condition | LED RGB | Buzzer | Servo | Ventilateur |
|------|-----------|---------|--------|-------|-------------|
| **Veille** | Lumière ≥ seuil, pas de vibration |  Verte | Silence | 0° | Arrêté |
| **Alerte** | Lumière < seuil OU vibration détectée | Rouge | 1000 Hz | 90° | Actif |
| **Désarmé** | Commande IR désarmement reçue | Bleue | Silence | 0° | Arrêté |

### Logique de déclenchement

- Le **potentiomètre** (A1) règle le seuil en temps réel via `map()` (plage : 200–800).
- Les deux capteurs (LDR + vibration) sont en logique **OU** : l'un ou l'autre suffit à déclencher l'alerte.
- Le flag `inAlert` garantit que les commandes servo et moteur ne sont envoyées qu'une seule fois au changement d'état, pas à chaque itération.
- La télécommande IR est vérifiée à **chaque itération** de `loop()` avec `irReceiver.decode()`, qui est non-bloquant (retourne `false` si aucun signal n'est arrivé).

---

##  Matériel utilisé

| Composant | Quantité | Broche Arduino |
|-----------|----------|---------------|
| Arduino Uno | 1 | — |
| LED RGB (cathode commune) | 1 | R→9, G→10, B→11 |
| Résistances 220 Ω | 3 | En série avec LED RGB |
| Buzzer piézoélectrique | 1 | 8 |
| LDR | 1 | A0 |
| Résistance 10 kΩ | 1 | Diviseur de tension LDR |
| Potentiomètre 10 kΩ | 1 | A1 |
| Servomoteur SG90 | 1 | 6 |
| Capteur de vibration | 1 | 2 (interrupt) |
| Transistor NPN 2N2222 | 1 | Base → 5 |
| Moteur DC (ventilateur) | 1 | Via transistor |
| Récepteur IR TSOP4838 | 1 | 7 |
| Télécommande IR | 1 | — |
| Breadboard | 1 | — |
| Câbles de connexion | ~25 | — |
| Câble USB | 1 | — |

---

##  Bibliothèques utilisées

| Bibliothèque | Usage | Installation |
|---|---|---|
| `Servo.h` | Contrôle du servomoteur | Incluse dans Arduino IDE |
| `IRremote` | Réception des signaux IR | Arduino IDE → Gestionnaire de bibliothèques → *IRremote* by shirriff |

---

##  Composants substitués

*(Remplir uniquement si un composant a été remplacé.)*

Exemple :
> **Transistor 2N2222** remplacé par **BC547** — même famille NPN, paramètres compatibles pour ce niveau de courant.

---

##  Difficultés rencontrées

- **Trouver les codes IR de la télécommande :** la bibliothèque IRremote envoie `0xFFFFFFFF` lors des appuis longs (signal de répétition). Solution : filtrer ce code dans `handleIR()` avant toute comparaison.

- **Jitter du servomoteur :** sans le flag `inAlert`, `alertServo.write(90)` était appelé à chaque itération de `loop()` pendant toute la durée de l'alerte, causant des micro-vibrations. Solution : n'envoyer la commande qu'une seule fois au moment du changement d'état.

- **Vibration volatile :** le mot-clé `volatile` était manquant sur `vibrationDetected` lors du premier test. Sans lui, le compilateur optimisait la variable en la mettant en cache et la valeur écrite par l'ISR n'était jamais vue par `loop()`.

*(Compléter avec vos propres difficultés réelles rencontrées pendant la semaine.)*

---

## 📚 Sources consultées

- [Arduino Reference — analogRead()](https://www.arduino.cc/reference/en/language/functions/analog-io/analogread/)
- [Arduino Reference — analogWrite()](https://www.arduino.cc/reference/en/language/functions/analog-io/analogwrite/)
- [Arduino Reference — tone()](https://www.arduino.cc/reference/en/language/functions/advanced-io/tone/)
- [Arduino Reference — Servo library](https://www.arduino.cc/reference/en/libraries/servo/)
- [Arduino Example — Knob (servo + potentiomètre)](https://docs.arduino.cc/built-in-examples/servo/Knob/)
- [Arduino Example — Tone Melody](https://docs.arduino.cc/built-in-examples/digital/toneMelody/)
- [IRremote library — GitHub](https://github.com/Arduino-IRremote/Arduino-IRremote)
- Document DFRobot — Projets 3 à 15

---

émonstration

Voir `demo.mp4` dans ce dossier.

La vidéo montre dans l'ordre :
1. Système en **veille** — LED verte, servo à 0°, silence
2. **Alerte par lumière** — couvrir le LDR avec la main
3. **Réinitialisation automatique** — retirer la main
4. **Alerte par vibration** — tapoter la breadboard
5. **Désarmement IR** — bouton télécommande → LED bleue
6. **Réarmement IR** — bouton télécommande → LED verte

---

*Projet réalisé dans le cadre du PÔLE ROS — FIERI | Semaine 2 | Mai 2026*

