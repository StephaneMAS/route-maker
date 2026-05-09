#include <Servo.h>
#include <IRremote.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ---------------- IR BUTTON CODES ----------------
// ⚠️ Replace these values with your REAL remote codes if needed
#define BTN1 0x16   // SYSTEM ON
#define BTN3 0x18   // SYSTEM OFF
#define BTN4 0x5E   // BUZZER OFF
#define BTN5 0x08   // LED OFF
#define BTN6 0x1C   // BUZZER TEST

unsigned long code;

// ---------------- PINS ----------------
int ledRouge = 9;
int ledVerte = 10;
int ledBleue = 11;

int buzzer = 8;

int pot = A1;
int ldr = A0;

int irPin = 7;
int trig = 2;
int echo = 4;

// ---------------- OBJECTS ----------------
Servo servo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ---------------- VARIABLES ----------------
int valLDR;
int valPOT;
float distance;

unsigned long lastTime = 0;
const long interval = 1000;

int etat = 0;              // 0 = NORMAL, 1 = ALERT
bool systemActive = true;

// ---------------- ULTRASONIC FUNCTION ----------------
float getDistance() {

  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  digitalWrite(trig, HIGH);
  delayMicroseconds(10);

  digitalWrite(trig, LOW);

  long duration = pulseIn(echo, HIGH, 30000);

  return duration * 0.034 / 2;
}

// ---------------- NORMAL MODE ----------------
void normal() {

  if (etat != 0) {

    etat = 0;

    // Green LED ON
    digitalWrite(ledVerte, HIGH);
    digitalWrite(ledRouge, LOW);
    digitalWrite(ledBleue, LOW);

    noTone(buzzer);

    servo.write(0);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SYSTEM NORMAL");
  }
}

// ---------------- ALERT MODE ----------------
void alerte() {

  if (etat != 1) {

    etat = 1;

    // Red LED ON
    digitalWrite(ledVerte, LOW);
    digitalWrite(ledRouge, HIGH);
    digitalWrite(ledBleue, LOW);

    tone(buzzer, 1000);

    servo.write(90);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SYSTEM ALERT");
  }
}

// ---------------- SYSTEM OFF ----------------
void systemeOFF() {

  systemActive = false;

  digitalWrite(ledRouge, LOW);
  digitalWrite(ledVerte, LOW);
  digitalWrite(ledBleue, LOW);

  noTone(buzzer);

  servo.write(0);

  lcd.clear();
  lcd.noBacklight();
}

// ---------------- SYSTEM ON ----------------
void systemeON() {

  systemActive = true;

  lcd.backlight();
  lcd.clear();

  normal();
}

// ---------------- SETUP ----------------
void setup() {

  pinMode(ledRouge, OUTPUT);
  pinMode(ledVerte, OUTPUT);
  pinMode(ledBleue, OUTPUT);

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  pinMode(buzzer, OUTPUT);

  Serial.begin(9600);

  // Start IR receiver
  IrReceiver.begin(irPin, ENABLE_LED_FEEDBACK);

  servo.attach(6);

  lcd.init();
  lcd.backlight();

  normal();
}

// ---------------- LOOP ----------------
void loop() {

  // Read sensors
  valLDR = analogRead(ldr);
  valPOT = analogRead(pot);
  distance = getDistance();

  // Filter wrong ultrasonic values
  if (distance <= 0 || distance > 400) {
    distance = 400;
  }

  // Serial debug every 1 second
  unsigned long now = millis();

  if (now - lastTime >= interval) {

    lastTime = now;

    Serial.print("LDR = ");
    Serial.println(valLDR);

    Serial.print("POT = ");
    Serial.println(valPOT);

    Serial.print("DIST = ");
    Serial.println(distance);
  }

  // ---------------- SYSTEM LOGIC ----------------
  if (systemActive) {

    // Alert condition
    if (valLDR > valPOT + 30 || distance < 15) {
      alerte();
    }

    // Return to normal condition
    else if (valLDR < valPOT + 10 && distance > 20) {
      normal();
    }
  }

  // ---------------- IR CONTROL ----------------
  if (IrReceiver.decode()) {

    code = IrReceiver.decodedIRData.command;
    IrReceiver.resume();

    // SYSTEM ON
    if (code == BTN1) {
      systemeON();
    }

    // SYSTEM OFF
    if (code == BTN3) {
      systemeOFF();
    }

    // BUZZER OFF
    if (code == BTN4) {
      noTone(buzzer);
      lcd.clear();
      lcd.print("BUZZER OFF");
    }

    // LED OFF
    if (code == BTN5) {
      digitalWrite(ledRouge, LOW);
      digitalWrite(ledVerte, LOW);
      digitalWrite(ledBleue, LOW);

      lcd.clear();
      lcd.print("LED OFF");
    }

    // BUZZER TEST
    if (code == BTN6) {
      tone(buzzer, 2000);
      delay(200);
      noTone(buzzer);
    }
  }

  delay(100);
}
