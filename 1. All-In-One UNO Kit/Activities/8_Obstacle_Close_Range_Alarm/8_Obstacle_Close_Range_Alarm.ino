/*
  =========================================================
   Arduino UNO - Ultrasonic Sensor + LCD + Relay + LED
  =========================================================

  COMPONENTS USED:
  ---------------------------------------------------------
  - 16x2 LCD (I2C, Address: 0x21)
  - Ultrasonic Sensor (HC-SR04)
  - Relay Module
  - LED

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

  Relay Module:
    IN  -> D4
    VCC -> 5V
    GND -> GND

  LED:
    Positive -> D10
    Negative -> GND (via resistor)

  WORKING:
  ---------------------------------------------------------
  - Measures distance using ultrasonic sensor
  - Displays comparison with threshold (30 cm) on LCD
  - If distance < 30 cm → LED & Relay ON
  - If distance ≥ 30 cm → LED & Relay OFF
*/

#include <Wire.h>                     // I2C communication library
#include "Adafruit_LiquidCrystal.h"  // LCD library
#include <HCSR04.h>                  // Ultrasonic sensor library

/*******************LCD Setup*******************/
// Create LCD object with I2C address 0x21
Adafruit_LiquidCrystal lcd(0x21);

// String of 16 spaces used for clearing LCD lines
String NULL_TXT = "                ";

// Function to print two lines on LCD
void LCD_print(String txt1, String txt2)
{
  // Clear first row
  lcd.setCursor(0, 0);
  lcd.print("                ");

  // Clear second row
  lcd.setCursor(0, 1);
  lcd.print("                ");

  // Print first line
  lcd.setCursor(0, 0);
  lcd.print(txt1);

  // Print second line
  lcd.setCursor(0, 1);
  lcd.print(txt2);
}

/*******************Ultrasonic Sensor*******************/
// Define trigger and echo pins
const byte triggerPin = 6;
const byte echoPin = 5;

// Create ultrasonic sensor object
UltraSonicDistanceSensor distanceSensor(triggerPin, echoPin);

/*******************Pins*******************/
// Relay control pin
int relayPin = 4;

// LED control pin
int LedPin = 10;

void setup() {
  // Start Serial communication for debugging
  Serial.begin(115200);

  // Initialize I2C communication (A4 = SDA, A5 = SCL)
  Wire.begin();

  // Initialize LCD (16 columns, 2 rows)
  lcd.begin(16, 2);

  // Turn ON LCD backlight
  lcd.setBacklight(HIGH);

  // Debug message
  Serial.println("LCD initialized at 0x21");

  // Set relay and LED pins as OUTPUT
  pinMode(relayPin, OUTPUT);
  pinMode(LedPin, OUTPUT);
}

void loop() {

  // Store last update time
  static unsigned long previousMillis = 0;

  // Interval for updating display and logic (500 ms)
  const long interval = 500;

  // Display static text with threshold value (30 cm)
  LCD_print("Distance    30", "");

  // Infinite loop inside loop()
  while (1)
  {
    // Measure distance in centimeters
    float distance = distanceSensor.measureDistanceCm();

    // If invalid reading (-1), skip this iteration
    if (distance == -1)
      continue;

    // Get current time
    unsigned long currentMillis = millis();

    // Check if interval has passed
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      // ==========================
      // CONDITION CHECK (THRESHOLD = 30 cm)
      // ==========================
      if ((int)distance >= 30)
      {
        // If distance is greater than or equal to 30 cm

        // Show ">=" symbol on LCD
        lcd.setCursor(9, 0);
        lcd.print(">=");

        // Turn OFF relay and LED
        digitalWrite(relayPin, LOW);
        digitalWrite(LedPin, LOW);
      }
      else
      {
        // If distance is less than 30 cm

        // Show "<" symbol on LCD
        lcd.setCursor(9, 0);
        lcd.print("< ");

        // Turn ON relay and LED
        digitalWrite(relayPin, HIGH);
        digitalWrite(LedPin, HIGH);
      }

      // Print distance value in Serial Monitor
      Serial.println((int)distance);
    }

    // Small delay to stabilize readings
    delay(100);
  }
}