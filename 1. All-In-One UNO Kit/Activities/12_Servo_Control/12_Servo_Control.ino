/*
  =========================================================
   Arduino UNO - Servo Motor Sweep Program
  =========================================================

  COMPONENT USED:
  ---------------------------------------------------------
  - Servo Motor

  CONNECTIONS:
  ---------------------------------------------------------
  Servo Motor:
    Red   -> 5V
    Brown/Black -> GND
    Yellow/Orange -> Digital Pin 9 (PWM)

  WORKING:
  ---------------------------------------------------------
  - Servo rotates from 0° to 180°
  - Then rotates back from 180° to 0°
  - This cycle repeats continuously
*/

#include <Servo.h>   // Include Servo library to control servo motor

// Create a Servo object to control a servo motor
Servo myservo;

// Variable to store current servo position (angle)
int pos = 0;

void setup() {

  // Attach servo to pin 9
  // 600–2520 µs defines pulse width range (for better calibration)
  myservo.attach(9, 600, 2520);
}

void loop() {

  // ==========================
  // ROTATE FROM 0° TO 180°
  // ==========================
  for (pos = 0; pos <= 180; pos += 1) {

    // Move servo to position 'pos'
    myservo.write(pos);

    // Wait 15 milliseconds for smooth movement
    delay(15);
  }

  // Wait for 2 seconds at 180°
  delay(2000);

  // ==========================
  // ROTATE FROM 180° TO 0°
  // ==========================
  for (pos = 180; pos >= 0; pos -= 1) {

    // Move servo to position 'pos'
    myservo.write(pos);

    // Wait 15 milliseconds for smooth movement
    delay(15);
  }

  // Wait for 2 seconds at 0°
  delay(2000);
}