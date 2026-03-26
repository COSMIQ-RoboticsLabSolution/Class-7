#include <Wire.h>                  // For I2C communication
#include <Adafruit_AHTX0.h>       // Library for DHT20 (AHT20) sensor
#include "Adafruit_LiquidCrystal.h" // Library for LCD display

#define DHT20_ADDRESS 0x38        // I2C address of DHT20 sensor
#define BUZZER_PIN 3              // Buzzer connected to pin 3

Adafruit_AHTX0 aht;               // Create sensor object
Adafruit_LiquidCrystal lcd(0x21); // LCD I2C address (0x21)

uint8_t count_DHT20 = 0;          // Counter (not used here, can be removed)

String NULL_TXT = "                "; // Empty string (16 spaces for LCD clear)

// Function to print 2 lines on LCD
void LCD_print(String txt1, String txt2) {
  lcd.setCursor(0, 0);           // Go to first row
  lcd.print("                "); // Clear first row

  lcd.setCursor(0, 1);           // Go to second row
  lcd.print("                "); // Clear second row

  lcd.setCursor(0, 0);           // Set cursor again
  lcd.print(txt1);               // Print first line

  lcd.setCursor(0, 1);
  lcd.print(txt2);               // Print second line
}

int LedPin = 10;                 // LED connected to pin 10
int buzzerPin = 3;               // Buzzer pin (same as defined above)

void setup() {
  Serial.begin(115200);          // Start serial communication

  // Initialize LCD (16x2)
  while (!lcd.begin(16, 2)) {    // Keep trying until LCD works
    Serial.println("Could not init backpack. Check wiring.");
    delay(50);
  }
  Serial.println("LCD init'd");

  Wire.begin();                  // Start I2C communication

  // Initialize DHT20 sensor at address 0x38
  if (!aht.begin(&Wire, 0, DHT20_ADDRESS)) {
    Serial.println("Could not find DHT20 at 0x38");
    while (1);                   // Stop program if sensor not found
  }
  Serial.println("DHT20 init'd at address 0x38");

  pinMode(LedPin, OUTPUT);       // Set LED pin as output
  pinMode(buzzerPin, OUTPUT);    // Set buzzer pin as output
}

void loop() {
  static bool LED_State = true;  // Used to blink LED (toggle state)

  sensors_event_t humidity, temp; // Create variables to store sensor data

  // Read humidity and temperature values
  aht.getEvent(&humidity, &temp);

  int value1 = humidity.relative_humidity; // Humidity value
  int value2 = temp.temperature;           // Temperature value

  // Print values on Serial Monitor
  Serial.print("Humidity: ");
  Serial.print(value1);
  Serial.print(" %\tTemp: ");
  Serial.print(value2);
  Serial.println(" C");

  // Create strings for LCD display
  String TemValue = "Tem:" + String(value2) + "C";
  String HumValue = "Hum:" + String(value1) + "%";

  // Temperature conditions
  if (value2 > 25 && value2 <= 30) {
    LCD_print("High Temperature", " ");
    digitalWrite(LedPin, HIGH);  // Turn LED ON
  }
  else if (value2 > 30) {
    LCD_print("Hot Temperature", " ");
    LED_State = !LED_State;      // Toggle LED state
    digitalWrite(LedPin, LED_State); // Blink LED
  }
  else {
    LCD_print(TemValue, HumValue); // Show normal values
    digitalWrite(LedPin, LOW);     // Turn LED OFF
  }

  // Humidity condition
  if (value1 < 40) {
    lcd.setCursor(0, 1);
    lcd.print("Dry Air");        // Show warning
    tone(buzzerPin, 1300);       // Turn buzzer ON (1300 Hz sound)
  } else {
    noTone(buzzerPin);           // Turn buzzer OFF
  }

  delay(1000); // Wait 1 second before next reading
}