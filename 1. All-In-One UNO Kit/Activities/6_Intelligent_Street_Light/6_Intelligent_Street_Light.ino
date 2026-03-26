/*
  =========================================================
   Arduino UNO - BH1750 Light Sensor with LED Control
  =========================================================

  LIBRARIES USED:
  ---------------------------------------------------------
  - Wire.h     → For I2C communication
  - BH1750.h   → For light sensor

  CONNECTIONS:
  ---------------------------------------------------------
  Arduino UNO I2C Pins:
    A4 -> SDA (Data line)
    A5 -> SCL (Clock line)

  BH1750 Light Sensor:
    VCC -> 5V
    GND -> GND
    SDA -> A4
    SCL -> A5
    I2C Address -> 0x5C

  LED:
    Positive (Long leg)  -> Digital Pin 10
    Negative (Short leg) -> GND (via 220Ω resistor)

  WORKING:
  ---------------------------------------------------------
  - Sensor measures ambient light intensity in lux
  - If light is LOW (≤ 100 lux) → LED turns ON
  - If light is HIGH (> 100 lux) → LED turns OFF
*/

// ==========================
// LIBRARIES
// ==========================
#include <Wire.h>        // I2C communication library
#include <BH1750.h>     // Light sensor library

// ==========================
// SENSOR OBJECT
// ==========================
BH1750 lightMeter(0x5c);   // Create BH1750 object with I2C address 0x5C

// ==========================
// VARIABLES
// ==========================
float lux;                 // Stores light intensity in lux

// ==========================
// LED PIN
// ==========================
int LedPin = 10;           // LED connected to digital pin 10

// ==========================
// SETUP FUNCTION
// ==========================
void setup() {

  // Start Serial Monitor for debugging
  Serial.begin(115200);

  // Initialize I2C communication (A4 = SDA, A5 = SCL)
  Wire.begin();

  // ==========================
  // SENSOR INITIALIZATION
  // ==========================
  // Start BH1750 in continuous high-resolution mode
  // This provides high accuracy (~1 lux resolution)
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x5c, &Wire)) {
    Serial.println(F("BH1750 Initialized Successfully"));
  } else {
    Serial.println(F("Error Initializing BH1750"));
  }

  // Set LED pin as OUTPUT
  pinMode(LedPin, OUTPUT);
}

// ==========================
// LOOP FUNCTION
// ==========================
void loop() {

  // ==========================
  // READ LIGHT VALUE
  // ==========================
  // Check if measurement is ready
  // 'true' makes it wait (blocking) until data is available
  if (lightMeter.measurementReady(true)) {

    // Read light intensity (in lux)
    lux = lightMeter.readLightLevel();

    // Print value to Serial Monitor
    Serial.print("Light Intensity: ");
    Serial.print(lux);
    Serial.println(" lx");
  }

  // Small delay for stability
  delay(10);

  // ==========================
  // LED CONTROL LOGIC
  // ==========================
  if (lux <= 100) {
    
    // Dark environment → Turn LED ON
    digitalWrite(LedPin, HIGH);

  } else {
    
    // Bright environment → Turn LED OFF
    digitalWrite(LedPin, LOW);
  }
}