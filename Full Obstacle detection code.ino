// Motor A (Left Side)
const int ENA = 11;
const int IN1 = 10;
const int IN2 = 9;
#define LEFT_SPEED 120  

// Motor B (Right Side)
const int ENB = 6;
const int IN3 = 8;
const int IN4 = 7;
#define RIGHT_SPEED 170

// Ultrasonic Sensor
const int TRIG = 2;
const int ECHO = 3;

// Distance threshold (cm)
#define STOP_DISTANCE 15  

void setup() {
  // Motor pin setup
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Ultrasonic pins
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  Serial.begin(9600);
}

void loop() {
  float distance = getDistance();
  Serial.print("Distance: ");
  Serial.println(distance);

  if (distance > STOP_DISTANCE) {
    moveForward();     // Clear → move
  } else {
    stopMotors();      // Obstacle → stop
  }

  delay(50);  // Small delay for stability
}

float getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH);
  float distance = duration * 0.034 / 2;  // Convert to cm
  return distance;
}

void moveForward() {
  // Left motor
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, LEFT_SPEED);

  // Right motor
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, RIGHT_SPEED);
}

void stopMotors() {
  // Left motor
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);

  // Right motor
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 0);
}

