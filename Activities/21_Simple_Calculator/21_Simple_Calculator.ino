#include <IRremote.h>              // Library for IR remote
#define IR_PIN 2                   // IR receiver connected to pin 2

#include <Wire.h>                  
#include "Adafruit_LiquidCrystal.h" // LCD library

Adafruit_LiquidCrystal lcd(0x21);  // LCD I2C address

// Strings to store input and split values
String num;     // Full expression (e.g. "12+34=")
String num1;    // First number
String num2;    // Second number
String num3;    // Result

int flag1 = 0;  // Check if '+' exists
int flag2 = 0;  // Check if '-' exists
long value = 0; // Final result

void setup() {

  Serial.begin(115200);            // Start Serial Monitor

  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK); // Start IR receiver

  Serial.println("init over");

  // Initialize LCD
  while (!lcd.begin(16, 2)) {
    Serial.println("Could not init backpack. Check wiring.");
    delay(50);
  }

  // Display heading
  lcd.setCursor(0, 0);
  lcd.print("Equal to:");
}

void loop() {

  // Check if any IR signal received
  if (IrReceiver.decode()) {

    byte code = IrReceiver.decodedIRData.command; // Get button code

    Serial.print("- press -\t");

    // Identify which button was pressed
    switch (code) {

      case 7: 
        Serial.println("[-]");
        num += '-';               // Add minus sign
        break;

      case 21: 
        Serial.println("[+]");
        num += '+';               // Add plus sign
        break;

      case 9: 
        Serial.println("[EQ]");
        num += '=';               // Add equal sign

        // Try to split and evaluate expression
        if (sliceString(num, num1, num2)) {

          // Perform operation based on flag
          if (flag1)
            value = performOperation(num1.toInt(), num2.toInt(), '+');
          else
            value = performOperation(num1.toInt(), num2.toInt(), '-');

          num = "";               // Clear input
          num3 = String(value);   // Convert result to string

          // Display result on LCD
          lcd.setCursor(0, 1);
          lcd.print("                "); // Clear line
          lcd.setCursor(0, 1);
          lcd.print(num3);

        } else {
          num = "";
          lcd.setCursor(0, 1);
          lcd.print("Error");     // Show error if invalid input
        }
        break;

      // Number buttons
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

    // Display current input on LCD (top row)
    lcd.setCursor(0, 0);
    lcd.print("                "); // Clear line
    lcd.setCursor(0, 0);
    lcd.print(num);

    IrReceiver.resume(); // Ready for next input
  }
}

/*************** FUNCTIONS ****************/

// Check if a character exists in string
bool containsCharacter(String str, char character) {
  return str.indexOf(character) != -1;
}

// Split string into two numbers based on operator
int sliceString(String input, String &part1, String &part2) {

  int opIndex;     // Position of operator (+ or -)
  int equalIndex;  // Position of '='

  // Check which operator is present
  flag1 = containsCharacter(input, '+');
  flag2 = containsCharacter(input, '-');

  // Ensure valid expression like "12+34="
  if ((flag1 || flag2) && containsCharacter(input, '=')) {

    if (flag1)
      opIndex = input.indexOf('+'); // Get '+' position
    else
      opIndex = input.indexOf('-'); // Get '-' position

    equalIndex = input.indexOf('=');

  } else {
    return 0; // Invalid expression
  }

  // Extract numbers from string
  part1 = input.substring(0, opIndex);
  part2 = input.substring(opIndex + 1, equalIndex);

  return 1; // Success
}

// Perform calculation
long performOperation(long a, long b, char operation) {

  switch (operation) {
    case '+': return a + b; // Addition
    case '-': return a - b; // Subtraction
    default: return a;      // Default case
  }
}