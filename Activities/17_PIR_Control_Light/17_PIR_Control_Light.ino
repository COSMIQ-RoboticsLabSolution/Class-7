#define PIR_PIN A2           // PIR motion sensor connected to analog pin A2 (used as digital input)

static int oldState = 0;     // Stores previous state of PIR sensor (0 = no motion, 1 = motion)
int LedPin = 10;             // LED connected to pin 10

void setup() {
  Serial.begin(115200);      // Start serial communication (for debugging/output)

  pinMode(PIR_PIN, INPUT);   // Set PIR pin as input
  pinMode(LedPin, OUTPUT);   // Set LED pin as output
}

void loop() {
  byte state = digitalRead(PIR_PIN); // Read current state of PIR sensor (HIGH or LOW)

  // If motion is detected AND previous state was different
  if (state && oldState != state) {
    Serial.println("[-] Motion detected!"); // Print message

    oldState = state;          // Update previous state

    digitalWrite(LedPin, HIGH); // Turn LED ON
    delay(5000);               // Keep LED ON for 5 seconds
  }

  // If NO motion detected AND previous state was different
  else if (!state && oldState != state) {
    Serial.println("[-] No Motion!"); // Print message

    digitalWrite(LedPin, LOW); // Turn LED OFF

    oldState = state;          // Update previous state
  }
}