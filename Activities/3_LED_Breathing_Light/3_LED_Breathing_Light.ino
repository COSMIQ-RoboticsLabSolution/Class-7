int LinearPin = A0;   // Potentiometer
int LedPin = 10;      // LED

int adcValue;
int mappedValue;

void setup() {
  pinMode(LedPin, OUTPUT);
  pinMode(LinearPin, INPUT);

  Serial.begin(115200);
}

void loop() {

  // Read potentiometer value (0–1023)
  adcValue = analogRead(LinearPin);

  // Convert to PWM range (0–255)
  mappedValue = map(adcValue, 0, 1023, 0, 255);

  // Control LED brightness
  analogWrite(LedPin, mappedValue);

  // Show value in Serial Monitor
  Serial.print("Pot Value: ");
  Serial.println(adcValue);

  delay(100); // 
}
