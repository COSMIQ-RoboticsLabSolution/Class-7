/*
  =========================================================
   Arduino UNO - MPU6050 Accelerometer + LCD Display System
  =========================================================

  COMPONENTS USED:
  ---------------------------------------------------------
  - MPU6050 (Accelerometer + Gyroscope)
  - 16x2 LCD (I2C)

  CONNECTIONS:
  ---------------------------------------------------------
  MPU6050:
    VCC -> 5V
    GND -> GND
    SDA -> A4
    SCL -> A5
    Address -> 0x68

  LCD (I2C):
    VCC -> 5V
    GND -> GND
    SDA -> A4
    SCL -> A5
    Address -> 0x21

  WORKING:
  ---------------------------------------------------------
  - Reads acceleration data from MPU6050 (X, Y, Z axes)
  - Converts raw values into m/s²
  - Displays values on Serial Monitor and LCD
*/

#include <Wire.h>                   // Library for I2C communication
#include <MPU6050.h>                // Library for MPU6050 sensor
#include "Adafruit_LiquidCrystal.h" // Library for LCD display

// ==========================
// CONSTANT DEFINITIONS
// ==========================
#define LCD_ADDRESS 0x21            // I2C address of LCD
#define MPU_ADDRESS 0x68            // I2C address of MPU6050

// ==========================
// OBJECT CREATION
// ==========================
MPU6050 mpu(MPU_ADDRESS);           // Create MPU6050 object
Adafruit_LiquidCrystal lcd(LCD_ADDRESS); // Create LCD object

// ==========================
// VARIABLE DECLARATIONS
// ==========================
int16_t ax, ay, az;                 // Raw acceleration values from sensor


// ==========================
// SETUP FUNCTION
// ==========================
void setup() {

  // Start I2C communication
  Wire.begin();

  // Start Serial communication for debugging
  Serial.begin(115200);

  // ==========================
  // PRINT DEVICE ADDRESSES
  // ==========================
  Serial.print("LCD Address: 0x");
  Serial.println(LCD_ADDRESS, HEX);

  Serial.print("MPU6050 Address: 0x");
  Serial.println(MPU_ADDRESS, HEX);


  // ==========================
  // MPU6050 INITIALIZATION
  // ==========================
  mpu.initialize();   // Initialize MPU6050 sensor

  // Verify sensor connection
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 not found at 0x68!");

    // Stop execution if sensor is not detected
    while (1);
  }

  Serial.println("MPU6050 connected!");


  // ==========================
  // LCD INITIALIZATION
  // ==========================
  while (!lcd.begin(16, 2)) {
    Serial.println("LCD not found at 0x21!");
    delay(100);
  }

  // ==========================
  // STARTUP DISPLAY
  // ==========================
  lcd.setCursor(0, 0);
  lcd.print("LCD:0x21 MPU:");

  lcd.setCursor(0, 1);
  lcd.print("Addr 0x68");

  delay(2000);        // Show message for 2 seconds
  lcd.clear();        // Clear LCD screen


  // ==========================
  // DISPLAY LABELS
  // ==========================
  lcd.setCursor(0, 0);
  lcd.print("ax:");   // X-axis label

  lcd.setCursor(8, 0);
  lcd.print("ay:");   // Y-axis label

  lcd.setCursor(0, 1);
  lcd.print("az:");   // Z-axis label
}


// ==========================
// LOOP FUNCTION
// ==========================
void loop() {

  // ==========================
  // READ SENSOR DATA
  // ==========================
  // Get raw acceleration values from MPU6050
  mpu.getAcceleration(&ax, &ay, &az);


  // ==========================
  // CONVERT TO PHYSICAL VALUES
  // ==========================
  // Convert raw data to acceleration (m/s²)
  // 16384 = sensitivity scale factor for ±2g range
  float fax = ax / 16384.0 * 9.8;
  float fay = ay / 16384.0 * 9.8;
  float faz = az / 16384.0 * 9.8;


  // ==========================
  // SERIAL OUTPUT
  // ==========================
  Serial.print("X: ");
  Serial.print(fax);

  Serial.print(" Y: ");
  Serial.print(fay);

  Serial.print(" Z: ");
  Serial.println(faz);


  // ==========================
  // LCD DISPLAY - X AXIS
  // ==========================
  lcd.setCursor(3, 0);
  lcd.print("     ");     // Clear previous value

  lcd.setCursor(3, 0);
  lcd.print(fax, 2);      // Display with 2 decimal precision


  // ==========================
  // LCD DISPLAY - Y AXIS
  // ==========================
  lcd.setCursor(11, 0);
  lcd.print("     ");

  lcd.setCursor(11, 0);
  lcd.print(fay, 2);


  // ==========================
  // LCD DISPLAY - Z AXIS
  // ==========================
  lcd.setCursor(3, 1);
  lcd.print("     ");

  lcd.setCursor(3, 1);
  lcd.print(faz, 2);


  // Small delay for smooth updates and readability
  delay(300);
}