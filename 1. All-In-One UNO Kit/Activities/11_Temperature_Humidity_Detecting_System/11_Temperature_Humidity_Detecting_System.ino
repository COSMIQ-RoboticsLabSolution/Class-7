/*
  =========================================================
   Arduino UNO + DHT20 + 16x2 LCD I2C
   LCD Library Used: Adafruit_LiquidCrystal
   DHT Library Used: Adafruit_AHTX0
  =========================================================

  CONNECTIONS:
  ---------------------------------------------------------
  Arduino UNO I2C Pins:
    A4 -> SDA
    A5 -> SCL

  DHT20 Sensor:
    VCC -> 5V
    GND -> GND
    SDA -> A4
    SCL -> A5
    I2C Address -> 0x38

  16x2 LCD Display:
    VCC -> 5V
    GND -> GND
    SDA -> A4
    SCL -> A5
    I2C Address -> 0x21

  DISPLAY:
  ---------------------------------------------------------
  Row 1 -> Temperature
  Row 2 -> Humidity
*/

#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include "Adafruit_LiquidCrystal.h"

// ==========================
// I2C ADDRESSES
// ==========================
#define DHT20_ADDRESS 0x38
#define LCD_ADDRESS   0x21

// ==========================
// LCD OBJECT
// ==========================
Adafruit_LiquidCrystal lcd(LCD_ADDRESS);

// ==========================
// DHT20 OBJECT
// ==========================
Adafruit_AHTX0 dht20;
sensors_event_t humidity, temp;

void setup() {
  Serial.begin(115200);

  // Start I2C communication
  Wire.begin();   // Arduino UNO: A4 = SDA, A5 = SCL

  // ==========================
  // LCD INITIALIZATION
  // ==========================
  while (!lcd.begin(16, 2)) {
    Serial.println("Could not init LCD backpack. Check wiring.");
    delay(50);
  }

  Serial.println("LCD Initialized");

  lcd.setCursor(0, 0);
  lcd.print("Temp & Humidity");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();

  // ==========================
  // DHT20 INITIALIZATION
  // ==========================
  if (!dht20.begin(&Wire, 0, DHT20_ADDRESS)) {
    Serial.println("DHT20 not found!");

    lcd.setCursor(0, 0);
    lcd.print("DHT20 ERROR");
    lcd.setCursor(0, 1);
    lcd.print("Check sensor");
    while (1);
  }

  Serial.println("DHT20 Initialized");
}

void loop() {
  // Read DHT20 data
  dht20.getEvent(&humidity, &temp);

  // ==========================
  // SERIAL MONITOR
  // ==========================
  Serial.print("Temperature: ");
  Serial.print(temp.temperature, 1);
  Serial.println(" C");

  Serial.print("Humidity: ");
  Serial.print(humidity.relative_humidity, 1);
  Serial.println(" %");

  // ==========================
  // LCD DISPLAY
  // ==========================

  // Row 1 -> Temperature
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp.temperature, 1);
  lcd.print((char)223);  // Degree symbol
  lcd.print("C   ");     // Extra spaces clear old values

  // Row 2 -> Humidity
  lcd.setCursor(0, 1);
  lcd.print("Hum : ");
  lcd.print(humidity.relative_humidity, 1);
  lcd.print("%   ");     // Extra spaces clear old values

  delay(2000);
}