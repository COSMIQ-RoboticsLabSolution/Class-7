/*
  =========================================================
   Arduino UNO - Potentiometer Controlled LED Brightness
  =========================================================

  CONNECTIONS:
  ---------------------------------------------------------
  POTENTIOMETER:
    Left Pin   -> 5V
    Right Pin  -> GND
    Middle Pin -> A0 (Analog Input)

  LED:
    Positive (Long leg)  -> Digital Pin 10 (PWM)
    Negative (Short leg) -> GND (via 220Ω resistor)

  WORKING:
  ---------------------------------------------------------
  - Potentiometer gives analog values (0–1023)
  - Arduino maps it to PWM range (0–255)
  - LED brightness changes accordingly
*/

// ==========================
// PIN DEFINITIONS
// ==========================
int LinearPin = A0;   // Potentiometer connected to analog pin A0
int LedPin = 10;      // LED connected to PWM pin D10

// ==========================
// VARIABLES
// ==========================
int adcValue;         // Stores raw analog value (0–1023)
int mappedValue;      // Stores mapped PWM value (0–255)

// ==========================
// SETUP FUNCTION
// ==========================
void setup() {

  // Set LED pin as OUTPUT
  pinMode(LedPin, OUTPUT);

  // Analog pins don’t strictly need pinMode, but added for clarity
  pinMode(LinearPin, INPUT);

  // Start Serial communication for debugging
  Serial.begin(115200);
}

// ==========================
// LOOP FUNCTION
// ==========================
void loop() {

  // Read analog value from potentiometer (0–1023)
  adcValue = analogRead(LinearPin);

  // Convert analog value to PWM range (0–255)
  mappedValue = map(adcValue, 0, 1023, 0, 255);

  // Apply PWM signal to LED (controls brightness)
  analogWrite(LedPin, mappedValue);

  // Print value to Serial Monitor
  Serial.print("Pot Value: ");
  Serial.println(adcValue);

  // Small delay for stability
  delay(100);
}