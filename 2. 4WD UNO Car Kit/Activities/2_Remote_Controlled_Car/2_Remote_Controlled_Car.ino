// =====================
// PIN CONFIGURATION
// =====================

// Motor Driver Pins (L298N or similar)
#define ENA 9    // Speed control for left motor
#define ENB 10   // Speed control for right motor
#define IN1 8    // Left motor direction pin 1
#define IN2 7    // Left motor direction pin 2
#define IN3 6    // Right motor direction pin 1
#define IN4 5    // Right motor direction pin 2

// IR Line Sensors (3 sensors)
#define LEFT_SENSOR 12    // Left sensor
#define CENTER_SENSOR 4   // Center sensor
#define RIGHT_SENSOR 2    // Right sensor

// =====================
// VARIABLES
// =====================

// Motor speeds (0–255)
int speedA = 140;   // Left motor speed
int speedB = 140;   // Right motor speed

// Stores last direction taken (F = forward, L = left, R = right)
char lastDirection = 'F';

// Time tracking for line detection
unsigned long lastSeenLine = 0;   // Last time line was detected
int lineLostDelay = 700;          // Delay before stopping (in ms)

// =====================
// SETUP FUNCTION (runs once)
// =====================
void setup() {

  // Motor pins setup
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Sensor pins setup
  pinMode(LEFT_SENSOR, INPUT);
  pinMode(CENTER_SENSOR, INPUT);
  pinMode(RIGHT_SENSOR, INPUT);

  Serial.begin(9600);  // Start serial monitor
}

// =====================
// MAIN LOOP (runs continuously)
// =====================
void loop() {

  // Read sensor values (HIGH = black line detected)
  int left = digitalRead(LEFT_SENSOR);
  int center = digitalRead(CENTER_SENSOR);
  int right = digitalRead(RIGHT_SENSOR);

  // Print sensor values for debugging
  Serial.print(left);
  Serial.print(" ");
  Serial.print(center);
  Serial.print(" ");
  Serial.println(right);

  // If ANY sensor detects the line → update last seen time
  if (left == HIGH || center == HIGH || right == HIGH) {
    lastSeenLine = millis();
  }

  // =====================
  // LINE FOLLOWING LOGIC
  // =====================

  // Case 1: Only center sees line → go straight
  if (center == HIGH && left == LOW && right == LOW) {
    forward();
    lastDirection = 'F';
  }

  // Case 2: Only left sees line → turn left
  else if (left == HIGH && center == LOW && right == LOW) {
    leftTurn();
    lastDirection = 'L';
  }

  // Case 3: Only right sees line → turn right
  else if (right == HIGH && center == LOW && left == LOW) {
    rightTurn();
    lastDirection = 'R';
  }

  // Case 4: Left + center → slight left correction
  else if (left == HIGH && center == HIGH && right == LOW) {
    slightLeft();
    lastDirection = 'L';
  }

  // Case 5: Right + center → slight right correction
  else if (right == HIGH && center == HIGH && left == LOW) {
    slightRight();
    lastDirection = 'R';
  }

  // Case 6: No sensor sees line
  else if (left == LOW && center == LOW && right == LOW) {

    // Check how long line is missing
    if (millis() - lastSeenLine > lineLostDelay) {
      stopCar();   // Line completely lost → stop
    } else {
      // Temporary loss → continue in last direction
      if (lastDirection == 'L') {
        leftTurn();
      }
      else if (lastDirection == 'R') {
        rightTurn();
      }
      else {
        forward();
      }
    }
  }

  // Default fallback → move forward
  else {
    forward();
  }
}

// =====================
// MOTOR CONTROL FUNCTIONS
// =====================

// Move forward
void forward() {
  analogWrite(ENA, speedA);
  analogWrite(ENB, speedB);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// Sharp left turn
void leftTurn() {
  analogWrite(ENA, 130);  // Slow left motor
  analogWrite(ENB, 160);  // Fast right motor

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// Sharp right turn
void rightTurn() {
  analogWrite(ENA, 160);  // Fast left motor
  analogWrite(ENB, 130);  // Slow right motor

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// Slight left adjustment
void slightLeft() {
  analogWrite(ENA, 110);
  analogWrite(ENB, 160);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// Slight right adjustment
void slightRight() {
  analogWrite(ENA, 160);
  analogWrite(ENB, 110);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// Stop the car completely
void stopCar() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}