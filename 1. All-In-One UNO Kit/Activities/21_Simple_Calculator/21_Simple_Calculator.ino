/*
  =========================================================
   Arduino UNO - IR Remote Calculator with LCD Display
  =========================================================

  COMPONENTS USED:
  ---------------------------------------------------------
  - IR Remote + IR Receiver
  - 16x2 LCD (I2C)

  CONNECTIONS:
  ---------------------------------------------------------
  IR Receiver:
    VCC -> 5V
    GND -> GND
    OUT -> Pin 2

  LCD (I2C):
    VCC -> 5V
    GND -> GND
    SDA -> A4
    SCL -> A5
    Address -> 0x21

  WORKING:
  ---------------------------------------------------------
  - Takes input from IR remote (numbers + operators)
  - Forms a mathematical expression (e.g., "12+34=")
  - Splits input into two numbers
  - Performs addition or subtraction
  - Displays result on LCD
*/

#include <IRremote.h>              // Library for IR remote communication
#define IR_PIN 2                   // IR receiver connected to digital pin 2

#include <Wire.h>                  
#include "Adafruit_LiquidCrystal.h" // LCD library

// ==========================
// OBJECT CREATION
// ==========================
Adafruit_LiquidCrystal lcd(0x21);  // Create LCD object (I2C address 0x21)


// ==========================
// VARIABLE DECLARATIONS
// ==========================
String num;     // Stores full input expression (e.g., "12+34=")
String num1;    // First number
String num2;    // Second number
String num3;    // Result (converted to string)

int flag1 = 0;  // Flag to check '+' operator
int flag2 = 0;  // Flag to check '-' operator

long value = 0; // Stores calculated result


// ==========================
// SETUP FUNCTION
// ==========================
void setup() {

  // Start Serial Monitor for debugging
  Serial.begin(115200);

  // Initialize IR receiver
  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);

  Serial.println("init over");

  // ==========================
  // LCD INITIALIZATION
  // ==========================
  while (!lcd.begin(16, 2)) {
    Serial.println("Could not init backpack. Check wiring.");
    delay(50);
  }

  // Display initial heading
  lcd.setCursor(0, 0);
  lcd.print("Equal to:");
}


// ==========================
// LOOP FUNCTION
// ==========================
void loop() {

  // Check if IR signal is received
  if (IrReceiver.decode()) {

    // Get the command code from remote
    byte code = IrReceiver.decodedIRData.command;

    Serial.print("- press -\t");

    // ==========================
    // BUTTON IDENTIFICATION
    // ==========================
    switch (code) {

      // Operator: Subtraction
      case 7: 
        Serial.println("[-]");
        num += '-';   // Append '-' to input string
        break;

      // Operator: Addition
      case 21: 
        Serial.println("[+]");
        num += '+';   // Append '+' to input string
        break;

      // Equal button → Evaluate expression
      case 9: 
        Serial.println("[EQ]");
        num += '=';   // Append '=' to complete expression

        // ==========================
        // PROCESS EXPRESSION
        // ==========================
        if (sliceString(num, num1, num2)) {

          // Perform operation based on detected operator
          if (flag1)
            value = performOperation(num1.toInt(), num2.toInt(), '+');
          else
            value = performOperation(num1.toInt(), num2.toInt(), '-');

          num = "";               // Clear input string
          num3 = String(value);   // Convert result to string

          // ==========================
          // DISPLAY RESULT ON LCD
          // ==========================
          lcd.setCursor(0, 1);
          lcd.print("                "); // Clear line

          lcd.setCursor(0, 1);
          lcd.print(num3);

        } else {

          // Invalid expression handling
          num = "";

          lcd.setCursor(0, 1);
          lcd.print("Error");
        }
        break;

      // ==========================
      // NUMBER BUTTONS (0–9)
      // ==========================
      case 22: Serial.println("[0]"); num += '0'; break;
      case 12: Serial.println("[1]"); num += '1'; break;
      case 24: Serial.println("[2]"); num += '2'; break;
      case 94: Serial.println("[3]"); num += '3'; break;
      case 8:  Serial.println("[4]"); num += '4'; break;
      case 28: Serial.println("[5]"); num += '5'; break;
      case 90: Serial.println("[6]"); num += '6'; break;
      case 66: Serial.println("[7]"); num += '7'; break;
      case 82: Serial.println("[8]"); num += '8'; break;
      case 74: Serial.println("[9]"); num += '9'; break;
    }

    // ==========================
    // DISPLAY INPUT ON LCD
    // ==========================
    lcd.setCursor(0, 0);
    lcd.print("                "); // Clear top row

    lcd.setCursor(0, 0);
    lcd.print(num); // Show current expression

    // Prepare IR receiver for next input
    IrReceiver.resume();
  }
}


// ==========================
// HELPER FUNCTIONS
// ==========================

// Check if a specific character exists in string
bool containsCharacter(String str, char character) {
  return str.indexOf(character) != -1;
}


// ==========================
// STRING SPLITTING FUNCTION
// ==========================
// Splits expression into two numbers
int sliceString(String input, String &part1, String &part2) {

  int opIndex;     // Index of operator (+ or -)
  int equalIndex;  // Index of '='

  // Check which operator exists
  flag1 = containsCharacter(input, '+');
  flag2 = containsCharacter(input, '-');

  // Validate expression format
  if ((flag1 || flag2) && containsCharacter(input, '=')) {

    if (flag1)
      opIndex = input.indexOf('+');
    else
      opIndex = input.indexOf('-');

    equalIndex = input.indexOf('=');

  } else {
    return 0; // Invalid expression
  }

  // Extract numbers using substring
  part1 = input.substring(0, opIndex);
  part2 = input.substring(opIndex + 1, equalIndex);

  return 1; // Successful split
}


// ==========================
// CALCULATION FUNCTION
// ==========================
long performOperation(long a, long b, char operation) {

  switch (operation) {

    case '+': return a + b; // Addition

    case '-': return a - b; // Subtraction

    default: return a;      // Default fallback
  }
}