
// STEP 1 — Read the light sensor (LDR)
// Goal: understand what analogRead() gives us and see values
//       change in real time in the Serial Monitor.
// Test: cover the LDR with your hand → value drops.
//       Shine a phone flashlight on it → value rises.


// LDR is on analog pin A0.
// We need an analog pin because the LDR produces a variable voltage
// (0 to 5V), not a simple HIGH/LOW. analogRead() converts that
// voltage into an integer between 0 (0V) and 1023 (5V).
const int PIN_LDR = A0;

void setup() {
  // Start serial communication at 9600 baud so we can print
  // values to the Serial Monitor (Tools → Serial Monitor).
  Serial.begin(9600);
  Serial.println("Step 1 ready — reading LDR...");
}

void loop() {
  // Read raw value from the LDR voltage divider circuit.
  // Bright light → high voltage → value close to 1023.
  // Darkness     → low voltage  → value close to 0.
  int lightValue = analogRead(PIN_LDR);

  Serial.print("Light value: ");
  Serial.println(lightValue);

  // 300ms pause so the Serial Monitor stays readable.
  // Without this, values scroll too fast to read.
  delay(300);
}
