#include <Wire.h>
#include "Adafruit_LiquidCrystal.h"
#include <HCSR04.h>

/*******************LCD Setup*******************/
// Explicitly define LCD with I2C address 0x21
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

/*******************Pins*******************/
int relayPin = 4;
int LedPin = 10;

void setup() {
  Serial.begin(115200);

  Wire.begin();

  // Initialize LCD
  lcd.begin(16, 2);
  lcd.setBacklight(HIGH);

  Serial.println("LCD initialized at 0x21");

  pinMode(relayPin, OUTPUT);
  pinMode(LedPin, OUTPUT);
}

void loop() {
  static unsigned long previousMillis = 0;
  const long interval = 500;

  LCD_print("Distance    30", "");

  while (1)
  {
    float distance = distanceSensor.measureDistanceCm();

    if (distance == -1)
      continue;

    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      if ((int)distance >= 30)
      {
        lcd.setCursor(9, 0);
        lcd.print(">=");
        digitalWrite(relayPin, LOW);
        digitalWrite(LedPin, LOW);
      }
      else
      {
        lcd.setCursor(9, 0);
        lcd.print("< ");
        digitalWrite(relayPin, HIGH);
        digitalWrite(LedPin, HIGH);
      }

      Serial.println((int)distance);
    }

    delay(100);
  }
}