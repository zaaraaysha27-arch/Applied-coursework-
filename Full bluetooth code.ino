#include <SoftwareSerial.h>

// Motor A (Left Side)
const int ENA = 11; // PWM pin for Motor A
const int IN1 = 10; // IN1 pin for Motor A
const int IN2 = 9;  // IN2 pin for Motor A
#define LEFT_SPEED 120  // Base speed for left motor

// Motor B (Right Side)
const int ENB = 6;  // PWM pin for Motor B
const int IN3 = 8;  // IN3 pin for Motor B
const int IN4 = 7;  // IN4 pin for Motor B
#define RIGHT_SPEED 170 // Slightly higher speed for right motor

// Bluetooth (SoftwareSerial)
SoftwareSerial bt(2, 3); // bt RX = pin2 (Arduino), bt TX = pin3 (Arduino via voltage divider)

// simple timing (not used heavily here)
unsigned long lastAction = 0;

void setup() {
  // Motor pins
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Serial for USB monitor
  Serial.begin(9600);
  // Bluetooth serial
  bt.begin(9600); // HC-05 default is usually 9600

  Serial.println("Ready. Waiting for BT commands...");
  bt.println("Connected to Arduino");
 
  // Start moving forward briefly as your original sketch did (optional)
  moveForward();
  delay(10000);
  stopMotors();
}

void loop() {
  // check for data from BT
  if (bt.available()) {
    char c = bt.read();
    Serial.print("BT received: ");
    Serial.println(c);

    // handle command
    if (c == 'F' || c == 'f') {
      moveForward();
      bt.println("Forward");
    } else if (c == 'B' || c == 'b') {
      moveBackward();
      bt.println("Backward");
    } else if (c == 'L' || c == 'l') {
      turnLeft();
      bt.println("Left");
    } else if (c == 'R' || c == 'r') {
      turnRight();
      bt.println("Right");
    } else if (c == 'S' || c == 's') {
      stopMotors();
      bt.println("Stopped");
    } else {
      bt.println("Unknown");
    }
  }

  // also check USB serial if you type into Serial Monitor (optional)
  if (Serial.available()) {
    char c = Serial.read();
    // echo to BT
    bt.write(c);
  }
}

void moveForward() {
  // Left motor (Motor A)
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, LEFT_SPEED);

  // Right motor (Motor B)
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, RIGHT_SPEED);
}

void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, LEFT_SPEED);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, RIGHT_SPEED);
}

void turnLeft() {
  // spin motors so robot turns left (adjust if your robot uses differential steering)
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, LEFT_SPEED / 2); // slow or reverse left wheel
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, RIGHT_SPEED);
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, LEFT_SPEED);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, RIGHT_SPEED / 2);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 0);
}
