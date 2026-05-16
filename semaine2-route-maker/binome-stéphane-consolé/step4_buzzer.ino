
// STEP 4 — Add the buzzer alarm
// Goal: trigger an audible alarm when alert condition is met.
// New concept: tone() / noTone() to generate sound on a pin.
// Test: cover LDR → LED turns red AND buzzer sounds.


const int PIN_LDR    = A0;
const int PIN_POT    = A1;
const int PIN_RED    = 9;
const int PIN_GREEN  = 10;
const int PIN_BLUE   = 11;
const int PIN_BUZZER = 8;   // Buzzer positive (+) → pin 8, negative → GND

// Alarm tone at 1000 Hz (1 kHz).
// We chose 1 kHz because it is clearly audible and sounds like
// a typical alarm, unlike very low (barely heard) or very high
// (uncomfortable) frequencies.
const int ALARM_FREQ = 1000;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_RED,    OUTPUT);
  pinMode(PIN_GREEN,  OUTPUT);
  pinMode(PIN_BLUE,   OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  Serial.println("Step 4 ready — buzzer added");
}

void loop() {
  int lightValue = analogRead(PIN_LDR);
  int threshold  = map(analogRead(PIN_POT), 0, 1023, 200, 800);

  if (lightValue < threshold) {
    // ALERT: red LED + buzzer alarm
    setRGB(255, 0, 0);
    tone(PIN_BUZZER, ALARM_FREQ);  // Start continuous tone at 1 kHz
    Serial.println(">> ALERT — buzzer ON");
  } else {
    // Normal: green LED + silence
    setRGB(0, 255, 0);
    noTone(PIN_BUZZER);            // Stop the tone
  }

  delay(200);
}

void setRGB(int r, int g, int b) {
  analogWrite(PIN_RED,   r);
  analogWrite(PIN_GREEN, g);
  analogWrite(PIN_BLUE,  b);
}
