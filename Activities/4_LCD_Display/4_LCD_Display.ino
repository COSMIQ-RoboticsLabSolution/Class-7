#include <Wire.h>
#include "Adafruit_LiquidCrystal.h"
// Use your specified I2C address
Adafruit_LiquidCrystal lcd(0x21);
void setup()
{
  Serial.begin(115200);
  // Initialize LCD
  while (!lcd.begin(16, 2))
  {
    Serial.println("Could not init backpack. Check wiring.");
    delay(50);
  }
  Serial.println("LCD Initialized");
  // Display text
  lcd.setCursor(0, 0);
  lcd.print("   WELCOME TO   ");
  lcd.setCursor(0, 1);
  lcd.print("   COSMIQ LAB   ");
  delay(10000);
  lcd.clear();
}
void loop(){
  
}



