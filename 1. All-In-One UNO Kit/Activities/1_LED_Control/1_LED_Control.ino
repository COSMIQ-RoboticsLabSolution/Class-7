/*
  =========================================================
   Arduino UNO - LED Blinking Program
  =========================================================

  CONNECTIONS:
  ---------------------------------------------------------
  LED:
    Positive (Long leg)  -> Digital Pin 10 (D10)
    Negative (Short leg) -> GND (via 220Ω resistor recommended)

  WORKING:
  ---------------------------------------------------------
  - The LED connected to pin D10 will blink continuously.
  - It will stay ON for 1 second and OFF for 1 second.
*/

// ==========================
// PIN DEFINITION
// ==========================
int LedPin = 10;   // LED is connected to digital pin 10

// ==========================
// SETUP FUNCTION (runs once)
// ==========================
void setup() {
  
  // Configure LED pin as OUTPUT
  // This allows Arduino to send HIGH (5V) or LOW (0V)
  pinMode(LedPin, OUTPUT);
}

// ==========================
// LOOP FUNCTION (runs forever)
// ==========================
void loop() {

  // Turn LED ON
  // HIGH means 5V is supplied → LED glows
  digitalWrite(LedPin, HIGH);

  // Wait for 1000 milliseconds (1 second)
  delay(1000);

  // Turn LED OFF
  // LOW means 0V → LED turns off
  digitalWrite(LedPin, LOW);

  // Wait again for 1 second
  delay(1000);
}