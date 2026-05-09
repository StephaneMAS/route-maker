#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ---------------- SENSORS ----------------
int ldr = A0;
int ldrVal;

int potentiometer = A1;
int limit;
int Limit;

int trig = 6;
int echo = 7;

// ---------------- LEDS ----------------
int rgb_red = 3;
int rgb_green = 4;
int led_radar = 2;

// ---------------- BUZZERS ----------------
int buzzer = 10;
int buzzer2 = 13;

// ---------------- SERVOS ----------------
Servo servoDoor;
Servo servoRadar;

// ---------------- RADAR BUTTON ----------------
int bpRadar = 12;
int etatbp;
int mem = HIGH;

bool radarEnabled = true;

// ---------------- RADAR ----------------
int radarPos = 90;
int radarDir = 1;

// ---------------- BUZZER 2 ----------------
unsigned long lastBuzz = 0;
bool buzzState = false;

// ---------------- SETUP ----------------
void setup() {

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  servoDoor.attach(9);
  servoRadar.attach(8);

  servoDoor.write(0);
  servoRadar.write(90);

  pinMode(rgb_red, OUTPUT);
  pinMode(rgb_green, OUTPUT);
  pinMode(led_radar, OUTPUT);

  pinMode(buzzer, OUTPUT);
  pinMode(buzzer2, OUTPUT);

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  pinMode(bpRadar, INPUT_PULLUP);
}

// ---------------- LOOP ----------------
void loop() {

  bool lightOK = normalLighting();
  bool intrus = intrusion();

  // ---------------- bouton radar ----------------
  etatbp = digitalRead(bpRadar);

  if (etatbp == LOW && mem == HIGH) {
    delay(30);
    if (digitalRead(bpRadar) == LOW) {
      radarEnabled = !radarEnabled;
    }
  }
  mem = etatbp;

  Serial.println(ldrVal);

  // ---------------- RADAR ----------------
  if (radarEnabled) {
    radarSweep(intrus);
  }
  else {
    digitalWrite(led_radar, LOW);
    servoRadar.write(90);
  }

  // ---------------- LIGHTING ----------------
  if (lightOK) {

    digitalWrite(rgb_green, HIGH);
    digitalWrite(rgb_red, LOW);

    // 🟢 door closed
    servoDoor.write(0);
  }
  else {

    digitalWrite(rgb_green, LOW);
    digitalWrite(rgb_red, HIGH);

    // 🔴 door open
    servoDoor.write(90);
  }

  // ---------------- INTRUSION ----------------
  if (intrus && radarEnabled) {

    digitalWrite(led_radar, HIGH);

    tone(buzzer, 1500);
  }
  else {

    digitalWrite(led_radar, LOW);
    noTone(buzzer);
  }

  // ---------------- BUZZER 2 (millis) ----------------
  if (digitalRead(rgb_red) == HIGH) {

    unsigned long now = millis();

    if (now - lastBuzz >= 300) {
      lastBuzz = now;
      buzzState = !buzzState;

      if (buzzState)
        tone(buzzer2, 2000);
      else
        noTone(buzzer2);
    }
  }
  else {
    noTone(buzzer2);
  }
}

// ---------------- LDR ----------------
bool normalLighting() {

  ldrVal = analogRead(ldr);
  limit = analogRead(potentiometer);
  Limit = map(limit, 0, 1023, 50, 1000);

  return ldrVal <= Limit;
}

// ---------------- ULTRASONIC ----------------
float readDistance() {

  digitalWrite(trig, LOW);
  delayMicroseconds(5);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  float duration = pulseIn(echo, HIGH, 30000);

  if (duration == 0) return -1;

  return 0.034 * duration / 2;
}

bool intrusion() {

  float d = readDistance();

  return (d > 0 && d <= 20);
}

// ---------------- RADAR ----------------
void radarSweep(bool intrus) {

  if (intrus) {
    servoRadar.write(radarPos);
    return;
  }

  servoRadar.write(radarPos);

  radarPos += radarDir;

  if (radarPos >= 150) radarDir = -1;
  if (radarPos <= 30)  radarDir = 1;

  delay(15);
}
