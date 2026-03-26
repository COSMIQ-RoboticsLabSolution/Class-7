#include <Wire.h>
#include "Adafruit_LiquidCrystal.h"
#include <HCSR04.h>

/*******************LCD Setup*******************/
// LCD pin intialization
Adafruit_LiquidCrystal lcd(0x21);

String NULL_TXT = "                ";

// Function to print two lines on LCD
void LCD_print(String txt1, String txt2)
{
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("                ");

  lcd.setCursor(0, 0);
  lcd.print(txt1);
  lcd.setCursor(0, 1);
  lcd.print(txt2);
}

/*******************Ultrasonic Sensor*******************/
const byte triggerPin = 6;
const byte echoPin = 5;
UltraSonicDistanceSensor distanceSensor(triggerPin, echoPin);

void setup() {
  Serial.begin(115200);

  Wire.begin();

  // Initialize LCD
  lcd.begin(16, 2);
  lcd.setBacklight(HIGH);

  Serial.println("LCD initialized at 0x21");
}

void loop() {
  static unsigned long previousMillis = 0;
  const long interval = 500;

  LCD_print("Distance:", "");

  while (1)
  {
    float distance = distanceSensor.measureDistanceCm();

    if (distance < 0) {
      continue;
    }

    String value = String((int)distance);

    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      lcd.setCursor(0, 1);
      lcd.print("     ");  // clear
      lcd.setCursor(0, 1);
      lcd.print(value);
    }

    delay(10);
  }
}