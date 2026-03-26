/*
  =========================================================
   Arduino UNO - PIR Motion Sensor with LED Indicator
  =========================================================

  COMPONENTS USED:
  ---------------------------------------------------------
  - PIR Motion Sensor
  - LED

  CONNECTIONS:
  ---------------------------------------------------------
  PIR Sensor:
    VCC -> 5V
    GND -> GND
    OUT -> A2 (used as digital input)

  LED:
    Positive -> Pin 10 (via resistor)
    Negative -> GND

  WORKING:
  ---------------------------------------------------------
  - Detects motion using PIR sensor
  - When motion is detected:
      → Prints "Motion detected!" on Serial Monitor
      → Turns LED ON for 5 seconds
  - When no motion is detected:
      → Prints "No Motion!"
      → Turns LED OFF
*/

#define PIR_PIN A2            // PIR sensor connected to analog pin A2 (used as digital input)

// ==========================
// VARIABLE DECLARATIONS
// ==========================
static int oldState = 0;      // Stores previous PIR state
                              // 0 = No motion, 1 = Motion detected

int LedPin = 10;              // LED connected to digital pin 10


// ==========================
// SETUP FUNCTION
// ==========================
void setup() {

  // Start Serial communication for monitoring output
  Serial.begin(115200);

  // Configure PIR sensor pin as INPUT
  pinMode(PIR_PIN, INPUT);

  // Configure LED pin as OUTPUT
  pinMode(LedPin, OUTPUT);
}


// ==========================
// LOOP FUNCTION
// ==========================
void loop() {

  // Read current state of PIR sensor
  // HIGH (1) → Motion detected
  // LOW  (0) → No motion
  byte state = digitalRead(PIR_PIN);


  // ==========================
  // MOTION DETECTED CONDITION
  // ==========================
  // Execute only when:
  // 1. Motion is detected (state = HIGH)
  // 2. Previous state was different (avoid repeated triggers)
  if (state && oldState != state) {

    Serial.println("[-] Motion detected!"); // Print message to Serial Monitor

    oldState = state;   // Update previous state to current

    digitalWrite(LedPin, HIGH); // Turn LED ON

    // Keep LED ON for 5 seconds
    delay(5000);
  }


  // ==========================
  // NO MOTION CONDITION
  // ==========================
  // Execute only when:
  // 1. No motion (state = LOW)
  // 2. Previous state was different
  else if (!state && oldState != state) {

    Serial.println("[-] No Motion!"); // Print message

    digitalWrite(LedPin, LOW); // Turn LED OFF

    oldState = state;   // Update previous state
  }
}