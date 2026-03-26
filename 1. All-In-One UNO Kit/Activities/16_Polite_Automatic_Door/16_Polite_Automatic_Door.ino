/*
  =========================================================
   Arduino UNO - Button Controlled Servo + LCD Display
  =========================================================

  COMPONENTS USED:
  ---------------------------------------------------------
  - Servo Motor
  - Push Button
  - 16x2 LCD (I2C, Address: 0x21)

  CONNECTIONS:
  ---------------------------------------------------------
  Servo Motor:
    VCC -> 5V
    GND -> GND
    Signal -> Pin 9

  Push Button:
    One side -> Pin 7
    Other side -> GND
    (Using INPUT mode → assumes external pull-up or wiring logic)

  LCD (I2C):
    VCC -> 5V
    GND -> GND
    SDA -> A4
    SCL -> A5
    Address -> 0x21

  WORKING:
  ---------------------------------------------------------
  - When button is pressed:
      → Servo rotates to 180°
      → LCD displays "Welcome"
  - When button is not pressed:
      → Servo returns to 0°
      → LCD is cleared
*/

#include <Servo.h>   // Library to control servo motors

// ==========================
// VARIABLE DECLARATIONS
// ==========================
int pos = 0;         // Variable for servo position (declared but not used here)

Servo myservo;       // Create Servo object

int buttonPin = 7;   // Digital pin where push button is connected


/*******************LCD FUNCTION SETUP*******************/
#include <Wire.h>                      // Library for I2C communication
#include "Adafruit_LiquidCrystal.h"   // Library for LCD display

Adafruit_LiquidCrystal lcd(0x21);     // Create LCD object with I2C address 0x21


// ==========================
// SETUP FUNCTION
// ==========================
void setup() {

  // Configure button pin as INPUT
  pinMode(buttonPin, INPUT);

  // Attach servo to pin 9 with custom pulse width range
  // 600 μs = minimum angle (0°)
  // 2520 μs = maximum angle (180°)
  myservo.attach(9, 600, 2520);

  // Start serial communication for debugging
  Serial.begin(115200);

  // ==========================
  // LCD INITIALIZATION
  // ==========================
  // Keep trying to initialize LCD until successful
  while (!lcd.begin(16, 2)) {
    Serial.println("Could not init backpack. Check wiring.");
    delay(50);
  }
}


// ==========================
// LOOP FUNCTION
// ==========================
void loop() {

  // ==========================
  // BUTTON CHECK
  // ==========================
  // digitalRead(buttonPin) returns:
  // HIGH  → button NOT pressed
  // LOW   → button pressed (active LOW configuration)
  if (!digitalRead(buttonPin)) {

    // ==========================
    // BUTTON PRESSED ACTION
    // ==========================

    myservo.write(180);   // Rotate servo to 180 degrees

    // Set cursor to first row, first column
    lcd.setCursor(0, 0);

    // Clear previous content by printing spaces
    lcd.print("             ");

    // Reset cursor position again
    lcd.setCursor(0, 0);

    // Display message
    lcd.print("Welcome");

  } else {

    // ==========================
    // BUTTON NOT PRESSED ACTION
    // ==========================

    myservo.write(0);     // Rotate servo back to 0 degrees

    // Clear LCD display
    lcd.setCursor(0, 0);
    lcd.print("             ");
  }

  // Small delay to:
  // 1. Reduce CPU usage
  // 2. Provide basic debounce for button
  delay(100);
}