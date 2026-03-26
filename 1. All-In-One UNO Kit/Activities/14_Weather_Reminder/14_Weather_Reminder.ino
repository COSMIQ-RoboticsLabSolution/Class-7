/*
  =========================================================
   Arduino UNO - DHT20 (AHT20) + LCD + LED + Buzzer System
  =========================================================

  COMPONENTS USED:
  ---------------------------------------------------------
  - DHT20 / AHT20 Temperature & Humidity Sensor
  - 16x2 LCD (I2C, Address: 0x21)
  - LED
  - Buzzer

  CONNECTIONS:
  ---------------------------------------------------------
  DHT20 Sensor:
    VCC -> 5V
    GND -> GND
    SDA -> A4
    SCL -> A5
    Address -> 0x38

  LCD (I2C):
    VCC -> 5V
    GND -> GND
    SDA -> A4
    SCL -> A5
    Address -> 0x21

  LED:
    Positive -> D10
    Negative -> GND (via resistor)

  Buzzer:
    Positive -> D3
    Negative -> GND

  WORKING:
  ---------------------------------------------------------
  - Reads temperature and humidity from DHT20 sensor
  - Displays values or warnings on LCD
  - Controls LED based on temperature
  - Activates buzzer when air is too dry
*/

#include <Wire.h>                    // Library for I2C communication
#include <Adafruit_AHTX0.h>         // Library for DHT20 (AHT20) sensor
#include "Adafruit_LiquidCrystal.h" // Library for LCD display

// ==========================
// CONSTANT DEFINITIONS
// ==========================
#define DHT20_ADDRESS 0x38          // I2C address of DHT20 sensor
#define BUZZER_PIN 3                // Buzzer connected to pin 3

// ==========================
// OBJECT CREATION
// ==========================
Adafruit_AHTX0 aht;                 // Sensor object
Adafruit_LiquidCrystal lcd(0x21);   // LCD object with I2C address 0x21

// Counter variable (not used in this program)
uint8_t count_DHT20 = 0;

// String of 16 spaces used to clear LCD rows
String NULL_TXT = "                ";

// ==========================
// LCD PRINT FUNCTION
// ==========================
// Prints two lines on LCD after clearing previous content
void LCD_print(String txt1, String txt2) {

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

// ==========================
// PIN DEFINITIONS
// ==========================
int LedPin = 10;      // LED connected to digital pin 10
int buzzerPin = 3;    // Buzzer connected to digital pin 3

void setup() {

  // Start Serial communication for debugging
  Serial.begin(115200);

  // ==========================
  // LCD INITIALIZATION
  // ==========================
  // Try initializing LCD until successful
  while (!lcd.begin(16, 2)) {
    Serial.println("Could not init backpack. Check wiring.");
    delay(50);
  }
  Serial.println("LCD init'd");

  // Start I2C communication
  Wire.begin();

  // ==========================
  // SENSOR INITIALIZATION
  // ==========================
  // Initialize DHT20 sensor with given I2C address
  if (!aht.begin(&Wire, 0, DHT20_ADDRESS)) {
    Serial.println("Could not find DHT20 at 0x38");

    // Stop execution if sensor not found
    while (1);
  }
  Serial.println("DHT20 init'd at address 0x38");

  // Set LED and buzzer pins as OUTPUT
  pinMode(LedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {

  // Variable used for LED blinking (toggle ON/OFF)
  static bool LED_State = true;

  // Create objects to store humidity and temperature data
  sensors_event_t humidity, temp;

  // ==========================
  // READ SENSOR DATA
  // ==========================
  aht.getEvent(&humidity, &temp);

  // Extract humidity and temperature values
  int value1 = humidity.relative_humidity; // Humidity (%)
  int value2 = temp.temperature;           // Temperature (°C)

  // ==========================
  // SERIAL OUTPUT
  // ==========================
  Serial.print("Humidity: ");
  Serial.print(value1);
  Serial.print(" %\tTemp: ");
  Serial.print(value2);
  Serial.println(" C");

  // ==========================
  // PREPARE LCD TEXT
  // ==========================
  String TemValue = "Tem:" + String(value2) + "C";
  String HumValue = "Hum:" + String(value1) + "%";

  // ==========================
  // TEMPERATURE CONDITIONS
  // ==========================
  if (value2 > 25 && value2 <= 30) {

    // Moderate high temperature
    LCD_print("High Temperature", " ");

    // Turn LED ON
    digitalWrite(LedPin, HIGH);
  }
  else if (value2 > 30) {

    // Very high temperature
    LCD_print("Hot Temperature", " ");

    // Toggle LED state to create blinking effect
    LED_State = !LED_State;
    digitalWrite(LedPin, LED_State);
  }
  else {

    // Normal temperature → show actual readings
    LCD_print(TemValue, HumValue);

    // Turn LED OFF
    digitalWrite(LedPin, LOW);
  }

  // ==========================
  // HUMIDITY CONDITION
  // ==========================
  if (value1 < 40) {

    // Dry air detected
    lcd.setCursor(0, 1);
    lcd.print("Dry Air");

    // Activate buzzer with 1300 Hz tone
    tone(buzzerPin, 1300);
  } else {

    // Normal humidity → turn buzzer OFF
    noTone(buzzerPin);
  }

  // Wait 1 second before next reading
  delay(1000);
}