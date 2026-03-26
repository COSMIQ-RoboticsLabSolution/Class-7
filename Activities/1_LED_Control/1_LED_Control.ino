int LedPin = 10; // LED pin no.​

void setup(){
pinMode(LedPin, OUTPUT); // set as output​
}
void loop() {
digitalWrite(LedPin, HIGH); // LED ON​
delay(1000); // 1 sec wait​
digitalWrite(LedPin, LOW); // LED OFF​
delay(1000); // 1000 = 1 sec wait​
}