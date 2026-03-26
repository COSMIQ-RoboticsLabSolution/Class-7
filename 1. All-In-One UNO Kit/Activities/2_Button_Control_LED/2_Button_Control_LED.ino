/*
  =========================================================
   Arduino UNO - Button Controlled LED
  =========================================================

  CONNECTIONS:
  ---------------------------------------------------------
  LED:
    Positive (Long leg)  -> Digital Pin 10 (D10)
    Negative (Short leg) -> GND (via 220Ω resistor)

  BUTTON:
    One side  -> Digital Pin 7 (D7)
    Other side -> GND

    NOTE:
    - This code assumes INPUT mode (external resistor setup).
    - If using INPUT_PULLUP, logic will be reversed.

  WORKING:
  ---------------------------------------------------------
  - When button is PRESSED → LED turns OFF
  - When button is RELEASED → LED turns ON
  - This happens because of the wiring logic used
*/

// ==========================
// PIN DEFINITIONS
// ==========================
int buttonPin = 7;   // Button connected to digital pin 7
int LedPin = 10;     // LED connected to digital pin 10

// ==========================
// SETUP FUNCTION (runs once)
// ==========================
void setup() {

  // Set LED pin as OUTPUT (to control LED ON/OFF)
  pinMode(LedPin, OUTPUT);

  // Set button pin as INPUT (to read button state)
  pinMode(buttonPin, INPUT);
}

// ==========================
// LOOP FUNCTION (runs forever)
// ==========================
void loop() {

  // Read the current state of the button
  // HIGH  → Button pressed (depending on wiring)
  // LOW   → Button not pressed
  int buttonState = digitalRead(buttonPin);

  // ==========================
  // CONDITION CHECK
  // ==========================
  if (buttonState == HIGH) {
    
    // Button is pressed
    // Turn LED OFF
    digitalWrite(LedPin, LOW);

  } else {
    
    // Button is not pressed
    // Turn LED ON
    digitalWrite(LedPin, HIGH);
  }

  // Small delay to stabilize reading (basic debouncing)
  delay(100);
}
