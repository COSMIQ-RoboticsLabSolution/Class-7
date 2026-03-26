/*
  =========================================================
   Arduino UNO - Servo Motor Sweep (0° to 180° and Back)
  =========================================================

  COMPONENT USED:
  ---------------------------------------------------------
  - Servo Motor (SG90 or similar)

  CONNECTIONS:
  ---------------------------------------------------------
  Servo Motor:
    Red   -> 5V
    Brown -> GND
    Yellow/Orange -> Pin 9 (PWM)

  WORKING:
  ---------------------------------------------------------
  - Servo rotates from 0° to 180° slowly
  - Pauses for 2 seconds
  - Then rotates back from 180° to 0°
  - Again pauses for 2 seconds
  - This cycle repeats continuously
*/

#include <Servo.h>   // Include Servo library to control servo motors

// ==========================
// OBJECT CREATION
// ==========================
Servo myservo;       // Create a Servo object to control one servo motor
                     // (Arduino UNO can control up to ~12 servos)

// ==========================
// VARIABLE DECLARATION
// ==========================
int pos = 0;         // Variable to store current servo position (0° to 180°)

// ==========================
// SETUP FUNCTION
// ==========================
void setup() {

  // Attach servo to pin 9
  // 600 -> minimum pulse width (in microseconds)
  // 2520 -> maximum pulse width
  // These values help calibrate full range (0° to 180°)
  myservo.attach(9, 600, 2520);
}

// ==========================
// LOOP FUNCTION (RUNS FOREVER)
// ==========================
void loop() {

  // ==========================
  // FORWARD ROTATION (0° → 180°)
  // ==========================
  for (pos = 0; pos <= 180; pos += 1) { 

    // Move servo to current position
    myservo.write(pos);  

    // Wait 15 milliseconds for smooth motion
    // Smaller delay = faster movement
    delay(15);           
  }

  // Pause for 2 seconds after reaching 180°
  delay(2000);

  // ==========================
  // REVERSE ROTATION (180° → 0°)
  // ==========================
  for (pos = 180; pos >= 0; pos -= 1) { 

    // Move servo back step-by-step
    myservo.write(pos);  

    // Wait 15 milliseconds for smooth return motion
    delay(15);           
  }

  // Pause for 2 seconds after returning to 0°
  delay(2000);
}