#include <BH1750.h>              // Library for light sensor
#include <Wire.h>                
#include "Adafruit_LiquidCrystal.h" // LCD library

#define BH1750_ADDRESS 0x5C      // I2C address of BH1750
#define LCD_ADDRESS    0x21      // I2C address of LCD

BH1750 lightMeter(BH1750_ADDRESS);        // Create light sensor object
Adafruit_LiquidCrystal lcd(LCD_ADDRESS);  // Create LCD object

#define PIR_PIN A2              // PIR motion sensor pin
#define SOUND_PIN A1            // Sound sensor pin
int LedPin = 10;                // LED pin

void setup() {
  Serial.begin(115200);        // Start Serial Monitor
  Wire.begin();                // Start I2C communication

  // Print device addresses (for debugging)
  Serial.print("BH1750 Address: 0x");
  Serial.println(BH1750_ADDRESS, HEX);

  Serial.print("LCD Address: 0x");
  Serial.println(LCD_ADDRESS, HEX);

  // Initialize LCD
  while (!lcd.begin(16, 2)) {
    Serial.println("LCD not found!");
    delay(100);
  }

  // Startup screen on LCD
  lcd.setCursor(0, 0);
  lcd.print("BH1750:0x5C");

  lcd.setCursor(0, 1);
  lcd.print("LCD:0x21");

  delay(2000);
  lcd.clear();

  // Initialize BH1750 light sensor
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, BH1750_ADDRESS, &Wire)) {
    Serial.println("BH1750 init OK");
  } else {
    Serial.println("BH1750 init FAIL");
  }

  pinMode(PIR_PIN, INPUT);     // Set PIR as input
  pinMode(SOUND_PIN, INPUT);   // Set sound sensor as input
  pinMode(LedPin, OUTPUT);     // Set LED as output
}

void loop() {

  // Check if new light data is available
  if (lightMeter.measurementReady(true)) {

    int lux = lightMeter.readLightLevel(); // Read light level in lux

    Serial.print("Light: ");
    Serial.print(lux);
    Serial.println(" lx");

    // Display light value on LCD
    lcd.setCursor(0, 0);
    lcd.print("Light:");

    lcd.setCursor(6, 0);
    lcd.print("      ");   // Clear old value

    lcd.setCursor(6, 0);
    lcd.print(lux);        // Print new value

    // If environment is DARK
    if (lux < 100) {

      // Stay in this loop until light becomes bright
      while (1) {

        int state = digitalRead(PIR_PIN); // Read motion sensor

        // If motion OR sound detected
        if (state == HIGH || digitalRead(SOUND_PIN)) {

          Serial.println("Motion/Sound!");
          digitalWrite(LedPin, HIGH); // Turn LED ON

          lcd.setCursor(0, 1);
          lcd.print("Detected     "); // Show detection

          delay(2000); // Keep LED ON for 2 sec
        } 
        else {
          Serial.println("No Motion");
          digitalWrite(LedPin, LOW); // Turn LED OFF

          lcd.setCursor(0, 1);
          lcd.print("No Motion    ");
        }

        // Recheck light level
        lux = lightMeter.readLightLevel();

        // Exit loop if environment becomes bright
        if (lux >= 100) break;
      }
    } 
    else {
      // If environment is BRIGHT
      digitalWrite(LedPin, LOW); // Keep LED OFF

      lcd.setCursor(0, 1);
      lcd.print("Bright Env   "); // Display message
    }
  }

  delay(100); // Small delay
}