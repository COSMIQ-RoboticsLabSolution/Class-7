#include <Wire.h>                 // I2C communication library
#include <MPU6050.h>              // MPU6050 sensor library
#include "Adafruit_LiquidCrystal.h" // LCD library

#define LCD_ADDRESS 0x21          // I2C address of LCD
#define MPU_ADDRESS 0x68          // I2C address of MPU6050

MPU6050 mpu(MPU_ADDRESS);        // Create MPU6050 object
Adafruit_LiquidCrystal lcd(LCD_ADDRESS); // Create LCD object

int16_t ax, ay, az;              // Raw acceleration values

void setup() {

  Wire.begin();                  // Start I2C communication
  Serial.begin(115200);          // Start Serial Monitor

  // Print device addresses to Serial Monitor (for debugging)
  Serial.print("LCD Address: 0x");
  Serial.println(LCD_ADDRESS, HEX);

  Serial.print("MPU6050 Address: 0x");
  Serial.println(MPU_ADDRESS, HEX);

  // Initialize MPU6050 sensor
  mpu.initialize();

  // Check if MPU6050 is connected properly
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 not found at 0x68!");
    while (1);                   // Stop program if sensor not found
  }

  Serial.println("MPU6050 connected!");

  // Initialize LCD (16x2)
  while (!lcd.begin(16, 2)) {
    Serial.println("LCD not found at 0x21!");
    delay(100);
  }

  // Display startup message on LCD
  lcd.setCursor(0, 0);
  lcd.print("LCD:0x21 MPU:");

  lcd.setCursor(0, 1);
  lcd.print("Addr 0x68");

  delay(2000);                  // Wait 2 seconds
  lcd.clear();                  // Clear screen

  // Print labels on LCD
  lcd.setCursor(0, 0);
  lcd.print("ax:");             // X-axis label

  lcd.setCursor(8, 0);
  lcd.print("ay:");             // Y-axis label

  lcd.setCursor(0, 1);
  lcd.print("az:");             // Z-axis label
}

void loop() {

  // Read acceleration values from MPU6050
  mpu.getAcceleration(&ax, &ay, &az);

  // Convert raw values to m/s²
  // 16384 = sensitivity scale factor for ±2g range
  float fax = ax / 16384.0 * 9.8;
  float fay = ay / 16384.0 * 9.8;
  float faz = az / 16384.0 * 9.8;

  // Print values to Serial Monitor
  Serial.print("X: ");
  Serial.print(fax);
  Serial.print(" Y: ");
  Serial.print(fay);
  Serial.print(" Z: ");
  Serial.println(faz);

  // Display X-axis value on LCD
  lcd.setCursor(3,0);          
  lcd.print("     ");           // Clear previous value
  lcd.setCursor(3,0);
  lcd.print(fax,2);             // Print with 2 decimal places

  // Display Y-axis value
  lcd.setCursor(11,0);
  lcd.print("     ");
  lcd.setCursor(11,0);
  lcd.print(fay,2);

  // Display Z-axis value
  lcd.setCursor(3,1);
  lcd.print("     ");
  lcd.setCursor(3,1);
  lcd.print(faz,2);

  delay(300);                  // Small delay for smooth update
}