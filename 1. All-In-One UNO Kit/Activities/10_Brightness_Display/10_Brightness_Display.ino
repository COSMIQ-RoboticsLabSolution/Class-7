/*
  =========================================================
   Arduino UNO - Potentiometer (Linear Sensor) + LCD + LED
  =========================================================

  COMPONENTS USED:
  ---------------------------------------------------------
  - Potentiometer (Linear Sensor)
  - 16x2 LCD (I2C, Address: 0x21)
  - LED (PWM control)

  CONNECTIONS:
  ---------------------------------------------------------
  Potentiometer:
    Left Pin   -> 5V
    Right Pin  -> GND
    Middle Pin -> A0

  LCD (I2C):
    VCC -> 5V
    GND -> GND
    SDA -> A4
    SCL -> A5

  LED:
    Positive -> D10 (PWM)
    Negative -> GND (via resistor)

  WORKING:
  ---------------------------------------------------------
  - Reads analog value (0–1023) from potentiometer
  - Maps value to PWM (0–255) to control LED brightness
  - Also maps value to (0–10) for simplified LCD display
*/

// ==========================
// PIN DEFINITIONS
// ==========================
int LinearPin = A0;  // Analog input pin for potentiometer
int LedPin = 10;     // PWM output pin for LED

/*******************LCD function*******************/
// Include required libraries for I2C LCD
#include <Wire.h>
#include "Adafruit_LiquidCrystal.h"

// Create LCD object with I2C address 0x21
Adafruit_LiquidCrystal lcd(0x21);

void setup() {

  // Start serial communication for debugging
  Serial.begin(115200);

  // ==========================
  // LCD INITIALIZATION
  // ==========================
  // Try initializing LCD until successful
  while (!lcd.begin(16, 2)) {
    Serial.println("Could not init backpack. Check wiring.");  // Error message
    delay(50);  // Retry after short delay
  }

  Serial.println("Backpack init'd.");  // Success message

  // Set LED pin as OUTPUT
  pinMode(LedPin, OUTPUT);

  // Set potentiometer pin as INPUT
  pinMode(LinearPin, INPUT);

  // Set cursor to first row
  lcd.setCursor(0, 0);

  // Display heading on LCD
  lcd.print("Brightness check");
}

void loop() {

  // Variable to store raw analog value (0–1023)
  int adcValue;

  // Variable to store mapped values
  int mappedValue;

  // ==========================
  // READ SENSOR VALUE
  // ==========================
  // Read analog value from potentiometer
  adcValue = analogRead(LinearPin);

  // ==========================
  // LED BRIGHTNESS CONTROL
  // ==========================
  // Map analog value (0–1023) to PWM range (0–255)
  mappedValue = map(adcValue, 0, 1023, 0, 255);

  // Apply PWM signal to LED (controls brightness)
  analogWrite(LedPin, mappedValue);

  // ==========================
  // LCD DISPLAY VALUE
  // ==========================
  // Map analog value to smaller range (0–10) for display
  mappedValue = map(adcValue, 0, 1023, 0, 10);

  // Convert integer value to String for LCD printing
  String Value = String(mappedValue);

  // Small delay for stable readings
  delay(100);

  // Move cursor to second row
  lcd.setCursor(0, 1);

  // Clear previous value by printing spaces
  lcd.print("    ");

  // Reset cursor to start of second row
  lcd.setCursor(0, 1);

  // Display mapped value (0–10) on LCD
  lcd.print(Value);
}