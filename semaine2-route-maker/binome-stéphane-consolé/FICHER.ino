// ================================================================
// STEP 2 — LDR + RGB LED (green = normal, red = alert)
// Goal: make the LED react to light with a fixed threshold.
// New concept: analogWrite() for PWM color control on RGB LED.
// Test: cover LDR → LED turns red. Uncover → LED turns green.
// ================================================================

const int PIN_LDR   = A0;

// RGB LED on PWM-capable pins 9, 10, 11.
// We chose PWM pins so analogWrite() lets us set any brightness
// level (0–255), not just full on or full off.
const int PIN_RED   = 9;
const int PIN_GREEN = 10;
const int PIN_BLUE  = 11;

// Fixed threshold for now — we will make it adjustable in Step 3.
// If lightValue drops below this, the system considers it an alert.
// Adjust this value after observing your LDR readings in Step 1.
const int FIXED_THRESHOLD = 400;

void setup() {
  Serial.begin(9600);

  // Declare LED pins as OUTPUT because we send signals to them.
  pinMode(PIN_RED,   OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE,  OUTPUT);

  Serial.println("Step 2 ready — LDR + RGB LED");
}

void loop() {
  int lightValue = analogRead(PIN_LDR);

  Serial.print("Light: ");
  Serial.print(lightValue);
  Serial.print(" | Threshold: ");
  Serial.println(FIXED_THRESHOLD);

  if (lightValue < FIXED_THRESHOLD) {
    // Light dropped below threshold → ALERT state
    // We write 255 to RED and 0 to the others to get pure red.
    setRGB(255, 0, 0);
    Serial.println(">> ALERT");
  } else {
    // Light is normal → STANDBY state
    setRGB(0, 255, 0);
    Serial.println(">> Normal");
  }

  delay(200);
}

// Helper function: set RGB LED to any color.
// Using analogWrite (PWM) gives us 256 levels per channel,
// which allows smooth color mixing if needed later.
void setRGB(int r, int g, int b) {
  analogWrite(PIN_RED,   r);
  analogWrite(PIN_GREEN, g);
  analogWrite(PIN_BLUE,  b);
}
