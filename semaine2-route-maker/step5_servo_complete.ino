
// STEP 5 — Add the servo motor + clean state management
// Goal: complete the mandatory project requirements.
//       Servo rotates to 90° on alert, returns on reset.
// New concept: Servo library, inAlert flag to avoid jitter.
// This step = 20/20 on the base score if well documented.

#include <Servo.h>

const int PIN_LDR    = A0;
const int PIN_POT    = A1;
const int PIN_RED    = 9;
const int PIN_GREEN  = 10;
const int PIN_BLUE   = 11;
const int PIN_BUZZER = 8;
const int PIN_SERVO  = 6;   // Servo signal (orange/yellow wire) → pin 6

const int ALARM_FREQ    = 1000;
const int SERVO_NORMAL  = 0;   // Resting angle
const int SERVO_ALERT   = 90;  // Alert angle

Servo alertServo;

// inAlert tracks whether we are currently in alert state.
// Without this flag, alertServo.write(90) would be called on every
// loop iteration, causing the servo to jitter and wear out quickly.
bool inAlert = false;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_RED,    OUTPUT);
  pinMode(PIN_GREEN,  OUTPUT);
  pinMode(PIN_BLUE,   OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  // Attach servo and send it to known resting position on startup.
  // This ensures a predictable physical state before the loop begins.
  alertServo.attach(PIN_SERVO);
  alertServo.write(SERVO_NORMAL);

  setNormalState();
  Serial.println("Step 5 ready — full base project");
}

void loop() {
  int lightValue = analogRead(PIN_LDR);
  int threshold  = map(analogRead(PIN_POT), 0, 1023, 200, 800);

  Serial.print("Light: "); Serial.print(lightValue);
  Serial.print(" | Threshold: "); Serial.println(threshold);

  if (lightValue < threshold) {
    if (!inAlert) {
      // State just became ALERT — execute one-time setup actions
      setAlertState();
      inAlert = true;
    }
    // Buzzer stays on continuously while alert is active
    tone(PIN_BUZZER, ALARM_FREQ);

  } else {
    if (inAlert) {
      // State just returned to NORMAL — reset everything
      setNormalState();
      inAlert = false;
    }
  }

  delay(100);
}

void setNormalState() {
  setRGB(0, 255, 0);
  noTone(PIN_BUZZER);
  alertServo.write(SERVO_NORMAL);
  Serial.println(">> Normal state");
}

void setAlertState() {
  setRGB(255, 0, 0);
  alertServo.write(SERVO_ALERT);
  Serial.println(">> ALERT state");
}

void setRGB(int r, int g, int b) {
  analogWrite(PIN_RED,   r);
  analogWrite(PIN_GREEN, g);
  analogWrite(PIN_BLUE,  b);
}
