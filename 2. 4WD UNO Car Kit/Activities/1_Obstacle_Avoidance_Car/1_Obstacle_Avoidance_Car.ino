#include <IRremote.hpp>   // Library for IR remote control (latest version)

// =====================
// PIN CONFIGURATION
// =====================

// IR Receiver Pin
#define IR_PIN 11   // IR receiver connected to pin 11

// Motor Driver Pins (L298N or similar)
#define ENA 9   // Enable motor A (left motor)
#define IN1 8   // Direction pin 1 for motor A
#define IN2 7   // Direction pin 2 for motor A

#define ENB 10  // Enable motor B (right motor)
#define IN3 6   // Direction pin 1 for motor B
#define IN4 5   // Direction pin 2 for motor B

// =====================
// IR REMOTE BUTTON CODES
// =====================
// These HEX values correspond to buttons on the remote

#define IR_FORWARD  0x18  // Button for forward movement
#define IR_BACKWARD 0x52  // Button for backward movement
#define IR_LEFT     0x08  // Button for left turn
#define IR_RIGHT    0x5A  // Button for right turn
#define IR_STOP     0x1C  // Button to stop

// =====================
// SETUP FUNCTION (runs once)
// =====================
void setup() {
  Serial.begin(9600);   // Start serial monitor for debugging

  // Start IR receiver
  // ENABLE_LED_FEEDBACK → LED blinks when signal is received
  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);

  // Motor pins setup
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Set motor speed (0–255)
  // Higher value = faster speed
  analogWrite(ENA, 200);
  analogWrite(ENB, 200);
}

// =====================
// MOTOR MOVEMENT FUNCTIONS
// =====================

// Move car forward
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// Move car backward
void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// Turn car left
void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// Turn car right
void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// Stop the car
void stopCar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// =====================
// MAIN LOOP (runs continuously)
// =====================
void loop() {

  // Check if IR signal is received
  if (IrReceiver.decode()) {

    // Get the command value from remote
    unsigned long cmd = IrReceiver.decodedIRData.command;

    // Print command in HEX format (for debugging)
    Serial.println(cmd, HEX);

    // Match button press with action
    if (cmd == IR_FORWARD) {
      moveForward();
    }
    else if (cmd == IR_BACKWARD) {
      moveBackward();
    }
    else if (cmd == IR_LEFT) {
      turnLeft();
    }
    else if (cmd == IR_RIGHT) {
      turnRight();
    }
    else if (cmd == IR_STOP) {
      stopCar();
    }

    // Prepare to receive next signal
    IrReceiver.resume();
  }
}