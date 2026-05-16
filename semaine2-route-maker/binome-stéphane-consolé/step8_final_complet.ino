
// STEP 8 — BONUS 3 : Arm / Disarm Button — FINAL VERSION
// Objective: a button allows the system to be armed or disarmed.
//            Short press → changes the state (armed ↔ disarmed).
//            Blue LED = system disarmed.
// New concept: debounce to prevent false button presses.
// Wiring: one button pin → pin 2, other button pin → GND.


#include <Servo.h>

// PINS

const int PIN_LDR    = A0;
const int PIN_POT    = A1;
const int PIN_RED    = 9;
const int PIN_GREEN  = 10;
const int PIN_BLUE   = 11;
const int PIN_BUZZER = 8;
const int PIN_SERVO  = 6;
const int PIN_MAT    = 4;   // Pressure mat button
const int PIN_TRIG   = 3;   // Ultrasonic TRIG
const int PIN_ECHO   = 5;   // Ultrasonic ECHO
const int PIN_ARM    = 2;   // Arm / Disarm button


// CONSTANTS

const int  ALARM_FREQ        = 1000;
const int  NORMAL_ANGLE      = 0;
const int  ALERT_ANGLE       = 90;
const int  DISTANCE_THRESHOLD = 50;   // cm
const long DEBOUNCE_DELAY    = 200;   // ms — minimum delay between valid presses


// STATE VARIABLES

bool  systemArmed = true;   // true = armed, false = disarmed
bool  inAlert     = false;

// Variables used for button debounce.
// A mechanical button generates several rapid transitions
// (bounces) when pressed. Without debounce, a single press
// could be interpreted as 5 to 20 presses.
long  lastPressTime = 0;
bool  previousButtonState = HIGH;


// OBJECTS

Servo alertServo;

void setup() {

  Serial.begin(9600);

  pinMode(PIN_RED,    OUTPUT);
  pinMode(PIN_GREEN,  OUTPUT);
  pinMode(PIN_BLUE,   OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  pinMode(PIN_MAT, INPUT_PULLUP);
  pinMode(PIN_ARM, INPUT_PULLUP);

  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  alertServo.attach(PIN_SERVO);
  alertServo.write(NORMAL_ANGLE);

  setNormalState();

  Serial.println("=== MONITORING STATION — FINAL VERSION ===");
  Serial.println("System ARMED. Monitoring active.");
  Serial.println("Press button on PIN 2 to disarm.");
  Serial.println("==========================================");
}

void loop() {

  // --- 1. Handle arm / disarm button ---
  handleArmButton();

  // --- 2. If disarmed: blue LED and ignore sensors ---
  if (!systemArmed) {

    setDisarmedState();

    delay(100);

    return;
  }

  // --- 3. Read sensors ---
  int  lightValue  = analogRead(PIN_LDR);

  int  threshold   = map(analogRead(PIN_POT), 0, 1023, 200, 800);

  bool matPressed  = (digitalRead(PIN_MAT) == LOW);

  long distance    = readDistance();

  Serial.print("Light : ");
  Serial.print(lightValue);

  Serial.print(" | Threshold : ");
  Serial.print(threshold);

  Serial.print(" | Mat : ");
  Serial.print(matPressed ? "YES" : "NO");

  Serial.print(" | Distance : ");
  Serial.print(distance);
  Serial.println(" cm");

  // --- 4. Evaluate alert condition ---
  // Three independent triggers — only one is enough.
  bool alertCondition =
        (lightValue < threshold)
     || matPressed
     || (distance > 0 && distance < DISTANCE_THRESHOLD);

  // --- 5. React ---
  if (alertCondition) {

    if (!inAlert) {

      setAlertState();

      inAlert = true;

      if (lightValue < threshold)
        Serial.println(">> Cause : LIGHT");

      if (matPressed)
        Serial.println(">> Cause : PRESSURE MAT");

      if (distance > 0 && distance < DISTANCE_THRESHOLD)
        Serial.println(">> Cause : ULTRASONIC SENSOR");
    }

    tone(PIN_BUZZER, ALARM_FREQ);

  } else {

    if (inAlert) {

      setNormalState();

      inAlert = false;
    }
  }

  delay(100);
}


// ARM BUTTON MANAGEMENT — with debounce

void handleArmButton() {

  bool currentState = digitalRead(PIN_ARM);

  // Detect only the FALLING edge (HIGH → LOW),
  // meaning the exact moment the button is pressed,
  // not while it is being held down.
  if (currentState == LOW && previousButtonState == HIGH) {

    // Debounce: check that at least
    // DEBOUNCE_DELAY ms passed since last valid press.
    if (millis() - lastPressTime > DEBOUNCE_DELAY) {

      systemArmed = !systemArmed;  // Toggle state

      lastPressTime = millis();

      if (systemArmed) {

        inAlert = false;

        setNormalState();

        Serial.println(">> Button : System ARMED");

      } else {

        noTone(PIN_BUZZER);

        Serial.println(">> Button : System DISARMED");
      }
    }
  }

  previousButtonState = currentState;
}


// ULTRASONIC MEASUREMENT

long readDistance() {

  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // pulseIn() measures echo duration in microseconds.
  // Dividing by 58 is a simplified version of:
  // (duration × 0.034 / 2)
  long duration = pulseIn(PIN_ECHO, HIGH, 30000);

  long distance = duration / 58;

  return distance;
}


// SYSTEM STATES

void setNormalState() {

  setRGB(0, 255, 0);   // Green = active standby

  noTone(PIN_BUZZER);

  alertServo.write(NORMAL_ANGLE);
}

void setAlertState() {

  setRGB(255, 0, 0);   // Red = alert

  alertServo.write(ALERT_ANGLE);
}

void setDisarmedState() {

  // Blue visually distinguishes the disarmed state
  // from normal standby (green) and alert mode (red).
  setRGB(0, 0, 255);

  noTone(PIN_BUZZER);

  alertServo.write(NORMAL_ANGLE);
}

void setRGB(int r, int g, int b) {

  analogWrite(PIN_RED,   r);

  analogWrite(PIN_GREEN, g);

  analogWrite(PIN_BLUE,  b);
}