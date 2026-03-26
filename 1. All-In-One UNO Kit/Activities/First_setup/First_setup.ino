
int functions_Number = 6;
/*******************DHT20*******************/





#include "DHT20.h"
DHT20 DHT(&Wire);
#define BUZZER_PIN 3
uint8_t count_DHT20 = 0;
/*******************Buzzer*******************/
int buzzerPin = 3;

/*******************Light*******************/
#include <BH1750.h>
BH1750 lightMeter(0x5c);
/**************************************/
#include "Adafruit_LiquidCrystal.h"
Adafruit_LiquidCrystal lcd(1);

String NULL_TXT = "                ";
void LCD_print(String txt1, String txt2)
{
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("                ");

  lcd.setCursor(0, 0);
  lcd.print(txt1);
  lcd.setCursor(0, 1);
  lcd.print(txt2);

}
/*******************Button*******************/
const int buttonPin = 7; //
int buttonState;         // 
int lastButtonState = LOW; // 
int count = 0;          //
int Button_ok = 0;
/*******************LED*******************/
int LedPin = 10;
/*******************Rotary*******************/
const int analogPin = A0; // 
int sensorValue_old = 0;
/*******************Ultrasonic*******************/
#include <HCSR04.h>

// Initialize sensor that uses digital pins 13 and 12.
const byte triggerPin = 6;
const byte echoPin = 5;
UltraSonicDistanceSensor distanceSensor(triggerPin, echoPin);

int Cursor_position = 0;
int flag = 0;
/*******************Servo*******************/
#include <Servo.h>
int pos = 0;
Servo myservo;
/*****************Sound************************/
#define SOUND_PIN A1

int Rotary_scanning() {

  int sensorValue = analogRead(analogPin); //  A0
  int i = 0;
  if (abs(sensorValue - sensorValue_old) > 100)
  {
    if (sensorValue_old + 50 > sensorValue )
    {
      Serial.println("Turn left");
      sensorValue_old = sensorValue;
      return 1;
    }
    else if (sensorValue_old + 50 < sensorValue )
    {
      Serial.println("Turn right");
      sensorValue_old = sensorValue;
      return 2;
    }

  } else
  {
    return 3;
  }

}

void UI_Update(int i)
{
  if (i == 0)
    LCD_print("*>LED   Light  *", "* THS   Sound  *");
  else if (i == 1)
    LCD_print("* LED  >Light  *", "* THS   Sound  *");
  else if (i == 2)
    LCD_print("* LED   Light  *", "*>THS   Sound  *");
  else if (i == 3)
    LCD_print("* LED   Light  *", "* THS  >Sound  *");
  else if (i == 4)
    LCD_print("*>Ultrasonic   *", "* Servo        *");
  else if (i == 5)
    LCD_print("* Ultrasonic   *", "*>Servo        *");
  else if (i == 6)
    LCD_print("*>Buzzer       *", "*              *");
}


void Button_scanning()
{
  buttonState = digitalRead(buttonPin); //

  // 
  if (buttonState == LOW && lastButtonState == HIGH) {
    count++; //
    Serial.print("Button pressed ");
    Serial.print(count);
    Serial.println(" times.");
    delay(200); //
    Button_ok = 1;
  }

  lastButtonState = buttonState; //
}



void LED_show()
{
  LCD_print("LED_show", "");
  unsigned long previousMillis = 0; //
  const long interval = 250;
  bool LED_State = true;
  while (1)
  {
    // 
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis; //
      LED_State = !LED_State ;
      digitalWrite(LedPin, LED_State);

    }


    Button_scanning();
    if (Button_ok == 1)
    {
      digitalWrite(LedPin, LOW);
      Button_ok = 0;
      UI_Update(Cursor_position);
      break;
    }
    delay(10);
  }


}
void Light_show()
{
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x5c, &Wire)) {
    Serial.println(F("BH1750 Advanced begin"));
  } else {
    Serial.println(F("Error initialising BH1750"));
  }
  float lux;

  unsigned long previousMillis = 0; //
  const long interval = 500;

  LCD_print("Light test:", "lux:");
  while (1)
  {
    if (lightMeter.measurementReady(true)) {
      lux = lightMeter.readLightLevel();
      Serial.print("[-] Light: [");
      Serial.print(lux);
      Serial.println("] lx");
    }
    delay(10);
    //
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis; //
      lcd.setCursor(5, 1);
      lcd.print(String(lux));
    }

    if (lux <= 100)
    {
      digitalWrite(LedPin, HIGH);
      lcd.setCursor(5, 1);
      lcd.print(String(lux));
    }
    else
      digitalWrite(LedPin, LOW);
    Button_scanning();
    if (Button_ok == 1)
    {
      Button_ok = 0;
      UI_Update(Cursor_position);
      break;
    }
  }

}
void THS_show()
{
  Wire.begin();
  DHT.begin();
  while (1)
  {
    if (millis() - DHT.lastRead() >= 1000) {
      //  READ DATA
      uint32_t start = micros();
      int status = DHT.read();
      uint32_t stop = micros();

      if ((count_DHT20 % 10) == 0) {
        count_DHT20 = 0;
        Serial.println();
        Serial.println("Type\tHumidity (%)\tTemp (°C)");
        //\tTime (µs)\tStatus
      }
      count_DHT20++;

      Serial.print("DHT20 \t");
      //  DISPLAY DATA, sensor has only one decimal.
      Serial.print(DHT.getHumidity(), 1);
      Serial.print("\t");
      Serial.print(DHT.getTemperature(), 1);

      Serial.print("\n");

      String TemValue = "Tem:" + String(DHT.getTemperature()) + "C";
      String HumValue = "Hum:" + String(DHT.getHumidity()) + "%";

      LCD_print(TemValue, HumValue);
    }

    Button_scanning();
    if (Button_ok == 1)
    {
      Button_ok = 0;
      UI_Update(Cursor_position);
      break;
    }

  }
}

void Ultrasonic_show()
{
  unsigned long previousMillis = 0; //
  const long interval = 500;
  LCD_print("Ultrasonic:", ""); //
  while (1)
  {
    float distance = distanceSensor.measureDistanceCm();
    String Value = String(distance);
    // 
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis; //
      lcd.setCursor(5, 1);
      lcd.print(Value);
    }

    delay(10);
    Serial.println((int)distance);
    if (distance <= 5)
    {
      tone(buzzerPin, 1300);
      digitalWrite(LedPin, HIGH);
    } else
    {
      noTone(buzzerPin);
      digitalWrite(LedPin, LOW);
    }

    Button_scanning();
    if (Button_ok == 1)
    {
      Button_ok = 0;
      UI_Update(Cursor_position);
      break;
    }
  }
  noTone(buzzerPin);

}
void Servo_show()
{
  myservo.attach(9, 400, 2550);
  int adcValue;
  int mappedValue;
  LCD_print("Servo_show", "Angle:");
  unsigned long previousMillis = 0; //
  const long interval = 500;
  while (1)
  {
    //    pos++;
    //    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    //    delay(15);                       // waits 15 ms for the servo to reach the position
    //    if (pos == 180)
    //      pos = 0;
    adcValue = analogRead(A0); // 
    mappedValue = map(adcValue, 0, 1023, 0, 180); // 
    Serial.println(adcValue);
    myservo.write(mappedValue);
    // 
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis; // 
      lcd.setCursor(6, 1);
      lcd.print("   ");
      lcd.setCursor(6, 1);
      lcd.print(mappedValue);
    }

    delay(10);
    Button_scanning();
    if (Button_ok == 1)
    {
      Button_ok = 0;
      UI_Update(Cursor_position);
      break;
    }
  }


}
void Buzzer_show()
{
  int adcValue;
  int mappedValue;
  LCD_print("Buzzer_show", "");
  while (1)
  {
    adcValue = analogRead(A0); //
    mappedValue = map(adcValue, 0, 1023, 0, 2700); //
    tone(buzzerPin, mappedValue);
    delay(10);
    Button_scanning();
    if (Button_ok == 1)
    {
      Button_ok = 0;
      noTone(buzzerPin);
      UI_Update(Cursor_position);
      break;
    }

  }

}
void Sound_show() {
LCD_print("Sound_show", "");
  while (1)
  {
    if ( digitalRead(SOUND_PIN) ) {
      Serial.println("[-] Detect Sound!");
      digitalWrite(LedPin, HIGH);
    }
    delay(20);
    digitalWrite(LedPin, LOW);
    Button_scanning();
    if (Button_ok == 1)
    {
      digitalWrite(LedPin, LOW);
      Button_ok = 0;
      UI_Update(Cursor_position);
      break;
    }

  }

}
void Selection_mode(int i)
{
  if (i == 0)
    LED_show();
  else if (i == 1)
    Light_show();
  else if (i == 2)
    THS_show();
  else if (i == 3)
    Sound_show();
  else if (i == 4)
    Ultrasonic_show();
  else if (i == 5)
    Servo_show();
  else if (i == 6)
    Buzzer_show();
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);   //
  while (!lcd.begin(16, 2)) {
    Serial.println("Could not init backpack. Check wiring.");
    delay(50);
  }
  Serial.println("Backpack init'd.");

  pinMode(buzzerPin, OUTPUT);

  // Print a message to the LCD.
  LCD_print("Start!", "");

  analogWrite(8, 100);
  //
  while (1)
  {
    Button_scanning();
    if (Button_ok == 1)
    {
      Button_ok = 0;
      break;
    }
    delay(10);
  }
  UI_Update(Cursor_position);
  //
  sensorValue_old = analogRead(analogPin);

  pinMode(SOUND_PIN, INPUT);
}


void loop() {
  // put your main code here, to run repeatedly:
  //  Rotary_scanning();
  flag = Rotary_scanning();
  delay(10);
  if (flag == 1)
  {
    if (Cursor_position == 0)
      Cursor_position = 0;
    else
      Cursor_position--;

    UI_Update(Cursor_position);
    Serial.println(Cursor_position);
  }
  else if (flag == 2)
  {
    if (Cursor_position == functions_Number)
      Cursor_position = functions_Number;
    else
      Cursor_position++;

    UI_Update(Cursor_position);
    Serial.println(Cursor_position);
  }
  Button_scanning();
  if (Button_ok == 1)
  {
    Button_ok = 0;
    Selection_mode(Cursor_position);
    sensorValue_old = analogRead(analogPin);
  }
}
