/*
  =========================================================
   Arduino UNO - Ultrasonic Distance + LCD Display
  =========================================================

  COMPONENTS USED:
  ---------------------------------------------------------
  - 16x2 LCD (I2C) → Address: 0x21
  - Ultrasonic Sensor (HC-SR04)

  CONNECTIONS:
  ---------------------------------------------------------
  LCD (I2C):
    VCC -> 5V
    GND -> GND
    SDA -> A4
    SCL -> A5

  Ultrasonic Sensor:
    VCC -> 5V
    GND -> GND
    TRIG -> D6
    ECHO -> D5

  WORKING:
  ---------------------------------------------------------
  - Measures distance using ultrasonic sensor
  - Displays distance on LCD
  - Updates every 500 ms
*/

#include <Wire.h>                     // Library for I2C communication
#include "Adafruit_LiquidCrystal.h"  // Library for LCD
#include <HCSR04.h>                  // Library for ultrasonic sensor

/*******************LCD Setup*******************/
// Create LCD object with I2C address 0x21
Adafruit_LiquidCrystal lcd(0x21);

// String containing 16 spaces (used to clear LCD lines)
String NULL_TXT = "                ";

// Function to print two lines on LCD
void LCD_print(String txt1, String txt2)
{
  // Move cursor to first row
  lcd.setCursor(0, 0);
  lcd.print("                ");   // Clear first row

  // Move cursor to second row
  lcd.setCursor(0, 1);
  lcd.print("                ");   // Clear second row

  // Print first line text
  lcd.setCursor(0, 0);
  lcd.print(txt1);

  // Print second line text
  lcd.setCursor(0, 1);
  lcd.print(txt2);
}

/*******************Ultrasonic Sensor*******************/
// Define trigger and echo pins
const byte triggerPin = 6;
const byte echoPin = 5;

// Create ultrasonic sensor object
UltraSonicDistanceSensor distanceSensor(triggerPin, echoPin);

void setup() {
  // Start serial communication for debugging
  Serial.begin(115200);

  // Initialize I2C communication (A4 = SDA, A5 = SCL)
  Wire.begin();

  // Initialize LCD (16 columns, 2 rows)
  lcd.begin(16, 2);

  // Turn ON LCD backlight
  lcd.setBacklight(HIGH);

  // Print confirmation in Serial Monitor
  Serial.println("LCD initialized at 0x21");
}

void loop() {

  // Store previous time for interval control
  static unsigned long previousMillis = 0;

  // Update interval (500 ms)
  const long interval = 500;

  // Display static label on LCD
  LCD_print("Distance:", "");

  // Infinite loop (runs continuously inside loop)
  while (1)
  {
    // Measure distance in centimeters
    float distance = distanceSensor.measureDistanceCm();

    // If invalid reading (negative), skip iteration
    if (distance < 0) {
      continue;
    }

    // Convert distance to integer and then to String
    String value = String((int)distance);

    // Get current time
    unsigned long currentMillis = millis();

    // Check if interval time has passed
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      // Clear part of second row
      lcd.setCursor(0, 1);
      lcd.print("     ");  

      // Set cursor again and print distance value
      lcd.setCursor(0, 1);
      lcd.print(value);
    }

    // Small delay for stability
    delay(10);
  }
}