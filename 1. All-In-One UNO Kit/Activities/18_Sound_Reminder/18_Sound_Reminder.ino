/*
  =========================================================
   Arduino UNO - Sound Sensor with Buzzer Alert System
  =========================================================

  COMPONENTS USED:
  ---------------------------------------------------------
  - Sound Sensor Module (Analog Output)
  - Buzzer

  CONNECTIONS:
  ---------------------------------------------------------
  Sound Sensor:
    VCC -> 5V
    GND -> GND
    OUT -> A1 (Analog Input)

  Buzzer:
    Positive -> Pin 3
    Negative -> GND

  WORKING:
  ---------------------------------------------------------
  - Continuously reads sound level from sensor
  - If sound level >= 400:
      → Prints "Detect Sound!" on Serial Monitor
      → Activates buzzer at 1300 Hz
      → Keeps checking sound level in a loop
  - When sound level drops below threshold:
      → Buzzer stops
      → System returns to normal monitoring
*/

#define SOUND_PIN A1        // Sound sensor connected to analog pin A1

// ==========================
// VARIABLE DECLARATIONS
// ==========================
int buzzerPin = 3;          // Buzzer connected to digital pin 3 (supports tone output)


// ==========================
// SETUP FUNCTION
// ==========================
void setup() {

  // Start serial communication for debugging
  Serial.begin(115200);

  // Wait until Serial Monitor is opened (useful for boards like Leonardo)
  while (!Serial);

  // Configure sound sensor pin as INPUT
  pinMode(SOUND_PIN, INPUT);

  // Configure buzzer pin as OUTPUT
  pinMode(buzzerPin, OUTPUT);
}


// ==========================
// LOOP FUNCTION
// ==========================
void loop() {

  // ==========================
  // SOUND DETECTION CHECK
  // ==========================
  // Read analog value from sound sensor (0–1023 range)
  // If value >= 400 → significant sound detected
  if (analogRead(SOUND_PIN) >= 400) {

    Serial.println("[-] Detect Sound!");  // Print detection message

    // Activate buzzer at 1300 Hz frequency
    tone(buzzerPin, 1300);

    // Keep buzzer ON for 1 second
    delay(1000);


    // ==========================
    // CONTINUOUS SOUND MONITORING
    // ==========================
    // Enter infinite loop to monitor if sound continues
    while (1) {

      // If sound is still above threshold
      if (analogRead(SOUND_PIN) >= 400) {

        // (Optional buzzer control is commented out in original code)
        // tone(buzzerPin, 1300);

        // Print current sound level for debugging/analysis
        Serial.println(analogRead(SOUND_PIN));
      }
      else {

        // Sound dropped below threshold → stop buzzer
        noTone(buzzerPin);

        // Exit infinite loop and return to main loop
        break;
      }
    }
  }
}