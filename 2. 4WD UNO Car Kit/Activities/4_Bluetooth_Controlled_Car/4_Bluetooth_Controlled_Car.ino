#define ENA 9
#define ENB 10
#define IN1 8
#define IN2 7
#define IN3 6
#define IN4 5

char command;

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.begin(9600);

  // Stop motors at start
  stopCar();
}

void loop() {
  if (Serial.available()) {
    command = Serial.read();

    if (command == 'F') {
      forward();
    }
    else if (command == 'B') {
      backward();
    }
    else if (command == 'L') {
      left();
    }
    else if (command == 'R') {
      right();
    }
    else if (command == 'S') {
      stopCar();
    }
  }
}

// ================= MOTOR FUNCTIONS =================

void forward() {
  analogWrite(ENA, 180);   // Speed motor A
  analogWrite(ENB, 180);   // Speed motor B

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void backward() {
  analogWrite(ENA, 180);
  analogWrite(ENB, 180);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void left() {
  analogWrite(ENA, 180);
  analogWrite(ENB, 180);

  // Left motor backward, right motor forward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void right() {
  analogWrite(ENA, 180);
  analogWrite(ENB, 180);

  // Left motor forward, right motor backward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopCar() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}