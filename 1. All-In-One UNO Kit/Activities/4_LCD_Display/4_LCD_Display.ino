/*
  =========================================================
   Arduino UNO - 16x2 LCD (I2C) Display Program
  =========================================================

  LIBRARIES USED:
  ---------------------------------------------------------
  - Wire.h                  → For I2C communication
  - Adafruit_LiquidCrystal  → For LCD control

  CONNECTIONS:
  ---------------------------------------------------------
  Arduino UNO I2C Pins:
    A4 -> SDA (Data line)
    A5 -> SCL (Clock line)

  16x2 LCD (I2C Module):
    VCC -> 5V
    GND -> GND
    SDA -> A4
    SCL -> A5

  I2C ADDRESS:
    LCD Address -> 0x21

  WORKING:
  ---------------------------------------------------------
  - Initializes the LCD using I2C communication
  - Displays a welcome message for 10 seconds
  - Clears the screen afterward
*/

// ==========================
// LIBRARIES
// ==========================
#include <Wire.h>                       // I2C communication library
#include "Adafruit_LiquidCrystal.h"    // LCD library

// ==========================
// LCD OBJECT (I2C ADDRESS)
// ==========================
Adafruit_LiquidCrystal lcd(0x21);      // LCD at I2C address 0x21

// ==========================
// SETUP FUNCTION
// ==========================
void setup()
{
  // Start Serial communication for debugging
  Serial.begin(115200);

  // Initialize I2C communication (uses A4 = SDA, A5 = SCL)
  Wire.begin();

  // ==========================
  // LCD INITIALIZATION
  // ==========================
  while (!lcd.begin(16, 2))   // 16 columns, 2 rows
  {
    Serial.println("Could not init backpack. Check wiring.");
    delay(50);  // Retry after short delay
  }

  Serial.println("LCD Initialized");

  // ==========================
  // DISPLAY MESSAGE
  // ==========================

  // Set cursor to column 0, row 0 (first line)
  lcd.setCursor(0, 0);
  lcd.print("   WELCOME TO   ");

  // Set cursor to column 0, row 1 (second line)
  lcd.setCursor(0, 1);
  lcd.print("   COSMIQ LAB   ");

  // Keep message visible for 10 seconds
  delay(10000);

  // Clear LCD screen
  lcd.clear();
}

// ==========================
// LOOP FUNCTION
// ==========================
void loop()
{
  // No repeated action required
}