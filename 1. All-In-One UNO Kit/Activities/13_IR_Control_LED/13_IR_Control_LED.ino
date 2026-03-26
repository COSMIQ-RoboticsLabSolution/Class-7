/*
  =========================================================
   Arduino UNO - IR Remote Controlled LED System
  =========================================================

  COMPONENTS USED:
  ---------------------------------------------------------
  - IR Receiver Module
  - IR Remote
  - LED

  CONNECTIONS:
  ---------------------------------------------------------
  IR Receiver:
    VCC -> 5V
    GND -> GND
    OUT -> D2

  LED:
    Positive -> D10 (via resistor)
    Negative -> GND

  WORKING:
  ---------------------------------------------------------
  - Receives IR signals from remote
  - Decodes button pressed
  - Performs different LED actions based on button
*/

#include <IRremote.h>   // Library for IR remote communication

// ==========================
// PIN DEFINITIONS
// ==========================
#define IR_PIN 2        // IR receiver signal pin connected to digital pin 2
#define LED_PIN 10      // LED connected to digital pin 10

void setup() {

  // Start serial communication for debugging
  Serial.begin(9600);

  // Set LED pin as OUTPUT
  pinMode(LED_PIN, OUTPUT);

  // ==========================
  // IR RECEIVER INITIALIZATION
  // ==========================
  // ENABLE_LED_FEEDBACK → onboard LED blinks when IR signal is received
  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);

  // Print system ready message
  Serial.println("IR LED Control Ready");
}

void loop() {

  // ==========================
  // CHECK FOR IR SIGNAL
  // ==========================
  if (IrReceiver.decode()) {

    // Extract command (button code) from received signal
    int code = IrReceiver.decodedIRData.command;

    // Print received button code
    Serial.print("Button Code: ");
    Serial.println(code);

    // ==========================
    // SWITCH CASE FOR BUTTONS
    // ==========================
    switch(code) {

      case 12: // Button "1"
        digitalWrite(LED_PIN, HIGH);   // Turn LED ON
        Serial.println("LED ON");
        break;

      case 24: // Button "2"
        digitalWrite(LED_PIN, LOW);    // Turn LED OFF
        Serial.println("LED OFF");
        break;

      case 94: // Button "3"
        // Toggle LED (ON ↔ OFF)
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
        Serial.println("LED TOGGLE");
        break;

      case 8: // Button "4" → slow blink
        digitalWrite(LED_PIN, HIGH);
        delay(500);                   // LED ON for 500 ms
        digitalWrite(LED_PIN, LOW);
        delay(500);                   // LED OFF for 500 ms
        break;

      case 28: // Button "5" → fast blink
        digitalWrite(LED_PIN, HIGH);
        delay(150);                   // LED ON for 150 ms
        digitalWrite(LED_PIN, LOW);
        delay(150);                   // LED OFF for 150 ms
        break;

      case 90: // Button "6"
        digitalWrite(LED_PIN, HIGH);  // LED ON
        break;

      case 66: // Button "7"
        digitalWrite(LED_PIN, LOW);   // LED OFF
        break;

      case 82: // Button "8"
        // Toggle LED state
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
        break;

      case 74: // Button "9" → medium blink
        digitalWrite(LED_PIN, HIGH);
        delay(300);                  // LED ON for 300 ms
        digitalWrite(LED_PIN, LOW);
        delay(300);                  // LED OFF for 300 ms
        break;

      case 22: // Button "0"
        digitalWrite(LED_PIN, LOW);   // LED OFF
        break;

      case 21: // "+" button
        digitalWrite(LED_PIN, HIGH);  // LED ON
        break;

      case 7: // "-" button
        digitalWrite(LED_PIN, LOW);   // LED OFF
        break;

      case 9: // "EQ" button
        // Toggle LED state
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
        Serial.println("EQ Toggle");
        break;
    }

    // ==========================
    // READY FOR NEXT SIGNAL
    // ==========================
    IrReceiver.resume();  // Reset receiver to accept next input
  }
}