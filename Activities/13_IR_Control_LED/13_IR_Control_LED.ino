#include <IRremote.h>   // Library for IR remote control

// Define pins
#define IR_PIN 2        // IR receiver signal pin connected to digital pin 2
#define LED_PIN 10      // LED connected to digital pin 10 (PWM supported)

void setup() {

  Serial.begin(9600);   // Start serial communication for debugging

  pinMode(LED_PIN, OUTPUT);  // Set LED pin as output

  // Initialize IR receiver
  // ENABLE_LED_FEEDBACK makes onboard LED blink when signal is received
  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);

  Serial.println("IR LED Control Ready");  // Status message
}

void loop() {

  // Check if IR signal is received
  if (IrReceiver.decode()) {

    // Extract the command (button code) from the received signal
    int code = IrReceiver.decodedIRData.command;

    // Print received button code in Serial Monitor
    Serial.print("Button Code: ");
    Serial.println(code);

    // Perform actions based on button pressed
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
        // Toggle LED state (ON -> OFF or OFF -> ON)
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
        Serial.println("LED TOGGLE");
        break;

      case 8: // Button "4" → slow blink
        digitalWrite(LED_PIN, HIGH);
        delay(500);                   // ON for 500ms
        digitalWrite(LED_PIN, LOW);
        delay(500);                   // OFF for 500ms
        break;

      case 28: // Button "5" → fast blink
        digitalWrite(LED_PIN, HIGH);
        delay(150);                   // ON for 150ms
        digitalWrite(LED_PIN, LOW);
        delay(150);                   // OFF for 150ms
        break;

      case 90: // Button "6"
        digitalWrite(LED_PIN, HIGH);  // LED ON
        break;

      case 66: // Button "7"
        digitalWrite(LED_PIN, LOW);   // LED OFF
        break;

      case 82: // Button "8"
        digitalWrite(LED_PIN, !digitalRead(LED_PIN)); // Toggle LED
        break;

      case 74: // Button "9" → medium blink
        digitalWrite(LED_PIN, HIGH);
        delay(300);
        digitalWrite(LED_PIN, LOW);
        delay(300);
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
        digitalWrite(LED_PIN, !digitalRead(LED_PIN)); // Toggle LED
        Serial.println("EQ Toggle");
        break;
    }

    // Prepare to receive the next IR signal
    IrReceiver.resume();
  }
}