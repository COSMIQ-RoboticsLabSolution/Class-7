/*
  =========================================================
   Arduino UNO - Smart Light + Motion + Sound Detection System
  =========================================================

  COMPONENTS USED:
  ---------------------------------------------------------
  - BH1750 Light Sensor
  - PIR Motion Sensor
  - Sound Sensor
  - LED
  - 16x2 LCD (I2C)

  CONNECTIONS:
  ---------------------------------------------------------
  BH1750:
    VCC -> 5V
    GND -> GND
    SDA -> A4
    SCL -> A5
    Address -> 0x5C

  LCD (I2C):
    VCC -> 5V
    GND -> GND
    SDA -> A4
    SCL -> A5
    Address -> 0x21

  PIR Sensor:
    OUT -> A2

  Sound Sensor:
    OUT -> A1

  LED:
    Positive -> Pin 10 (via resistor)
    Negative -> GND

  WORKING:
  ---------------------------------------------------------
  - Reads light intensity using BH1750 sensor
  - If environment is DARK (lux < 100):
      → Detects motion or sound
      → Turns LED ON and displays "Detected"
  - If environment is BRIGHT:
      → LED remains OFF
      → Displays "Bright Env"
*/

#include <BH1750.h>                 // Library for light sensor
#include <Wire.h>                  
#include "Adafruit_LiquidCrystal.h" // LCD library

// ==========================
// CONSTANT DEFINITIONS
// ==========================
#define BH1750_ADDRESS 0x5C        // I2C address of BH1750 sensor
#define LCD_ADDRESS    0x21        // I2C address of LCD

// ==========================
// OBJECT CREATION
// ==========================
BH1750 lightMeter(BH1750_ADDRESS);         // Create light sensor object
Adafruit_LiquidCrystal lcd(LCD_ADDRESS);   // Create LCD object

// ==========================
// PIN DEFINITIONS
// ==========================
#define PIR_PIN A2                // PIR motion sensor pin
#define SOUND_PIN A1              // Sound sensor pin
int LedPin = 10;                 // LED output pin


// ==========================
// SETUP FUNCTION
// ==========================
void setup() {

  // Start serial communication
  Serial.begin(115200);

  // Start I2C communication
  Wire.begin();

  // ==========================
  // DEBUG: PRINT DEVICE ADDRESSES
  // ==========================
  Serial.print("BH1750 Address: 0x");
  Serial.println(BH1750_ADDRESS, HEX);

  Serial.print("LCD Address: 0x");
  Serial.println(LCD_ADDRESS, HEX);


  // ==========================
  // LCD INITIALIZATION
  // ==========================
  while (!lcd.begin(16, 2)) {
    Serial.println("LCD not found!");
    delay(100);
  }

  // ==========================
  // STARTUP DISPLAY
  // ==========================
  lcd.setCursor(0, 0);
  lcd.print("BH1750:0x5C");

  lcd.setCursor(0, 1);
  lcd.print("LCD:0x21");

  delay(2000);   // Show startup info
  lcd.clear();   // Clear LCD


  // ==========================
  // BH1750 INITIALIZATION
  // ==========================
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, BH1750_ADDRESS, &Wire)) {
    Serial.println("BH1750 init OK");
  } else {
    Serial.println("BH1750 init FAIL");
  }

  // ==========================
  // PIN CONFIGURATION
  // ==========================
  pinMode(PIR_PIN, INPUT);       // Motion sensor as input
  pinMode(SOUND_PIN, INPUT);     // Sound sensor as input
  pinMode(LedPin, OUTPUT);       // LED as output
}


// ==========================
// LOOP FUNCTION
// ==========================
void loop() {

  // ==========================
  // CHECK IF NEW LIGHT DATA IS READY
  // ==========================
  if (lightMeter.measurementReady(true)) {

    // Read light intensity in lux
    int lux = lightMeter.readLightLevel();

    // ==========================
    // SERIAL OUTPUT
    // ==========================
    Serial.print("Light: ");
    Serial.print(lux);
    Serial.println(" lx");


    // ==========================
    // LCD DISPLAY - LIGHT VALUE
    // ==========================
    lcd.setCursor(0, 0);
    lcd.print("Light:");

    lcd.setCursor(6, 0);
    lcd.print("      ");   // Clear old value

    lcd.setCursor(6, 0);
    lcd.print(lux);        // Display new value


    // ==========================
    // DARK ENVIRONMENT LOGIC
    // ==========================
    if (lux < 100) {

      // Stay in this loop until environment becomes bright
      while (1) {

        // Read motion sensor
        int state = digitalRead(PIR_PIN);

        // ==========================
        // DETECTION CONDITION
        // ==========================
        // If motion OR sound is detected
        if (state == HIGH || digitalRead(SOUND_PIN)) {

          Serial.println("Motion/Sound!");

          digitalWrite(LedPin, HIGH); // Turn LED ON

          // Display detection message
          lcd.setCursor(0, 1);
          lcd.print("Detected     ");

          delay(2000); // Keep LED ON for 2 seconds
        } 
        else {

          Serial.println("No Motion");

          digitalWrite(LedPin, LOW); // Turn LED OFF

          lcd.setCursor(0, 1);
          lcd.print("No Motion    ");
        }

        // ==========================
        // RECHECK LIGHT LEVEL
        // ==========================
        lux = lightMeter.readLightLevel();

        // Exit loop if environment becomes bright
        if (lux >= 100) break;
      }
    } 
    else {

      // ==========================
      // BRIGHT ENVIRONMENT LOGIC
      // ==========================
      digitalWrite(LedPin, LOW); // Ensure LED is OFF

      lcd.setCursor(0, 1);
      lcd.print("Bright Env   "); // Display status
    }
  }

  // Small delay for stability
  delay(100);
}