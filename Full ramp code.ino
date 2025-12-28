#include <Wire.h>
#include <LiquidCrystal.h>
#include <math.h>

// LCD
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// MPU6050
const int MPU = 0x68;
float AccX, AccY, AccZ;
float rampAngle = 0;

// Motor A (Left)
const int ENA = A1;
const int IN1 = 12;
const int IN2 = 13;
#define LEFT_SPEED 200

// Motor B (Right)
const int ENB = 3;
const int IN3 = 2;
const int IN4 = 1;
#define RIGHT_SPEED 252

// State machine
enum RobotState {STATE_MOVE, STATE_WAIT_TOP, STATE_ROTATE, STATE_DONE};
RobotState state = STATE_MOVE;
unsigned long stateStartTime = 0;
bool rotationStarted = false;
unsigned long rotateStartTime = 0;

void setup() {
  lcd.begin(16, 2);
  lcd.print("Starting...");
  delay(500);

  // Motors
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  stopMotors();

  // MPU6050 setup
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);      
  Wire.write(0);          
  Wire.endTransmission(true);

  lcd.clear();
  lcd.print("Climbing Ramp");
  moveForward();
}

void loop() {
  unsigned long currentTime = millis();

  // ---- READ MPU6050 ----
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);

  AccX = Wire.read() << 8 | Wire.read();
  AccY = Wire.read() << 8 | Wire.read();
  AccZ = Wire.read() << 8 | Wire.read();

  // ---- CALCULATE PITCH ----
  float Ax = AccX / 16384.0;
  float Ay = AccY / 16384.0;
  float Az = AccZ / 16384.0;
  float pitch = -atan2(Ax, sqrt(Ay*Ay + Az*Az)) * 180 / PI;

  // ---- CALCULATE RAMP ANGLE DYNAMICALLY ----
  rampAngle = pitch; // current slope of ramp

  switch (state) {

    case STATE_MOVE:
      lcd.setCursor(0, 1);
      lcd.print("Angle:");
      lcd.print(rampAngle, 1);
      lcd.print("   ");

      // Detect flattening at top dynamically
      if (abs(rampAngle) < 5.0 && rampAngle > -5.0) { // ±5° threshold for top
        stopMotors();
        stateStartTime = currentTime;
        state = STATE_WAIT_TOP;
      }
      break;

    case STATE_WAIT_TOP:
      lcd.setCursor(0, 1);
      lcd.print("Angle:");
      lcd.print(rampAngle, 1);
      lcd.print("   ");

      // Wait 4 seconds
      if (currentTime - stateStartTime >= 4000) {
        state = STATE_ROTATE;
        rotationStarted = false;
      }
      break;

    case STATE_ROTATE:
      lcd.setCursor(0, 1);
      lcd.print("Rotating...     ");
      rotate360NonBlocking(currentTime);
      break;

    case STATE_DONE:
      lcd.setCursor(0, 1);
      lcd.print("Done!           ");
      stopMotors();
      break;
  }

  delay(50);
}

// ---------------------------
// MOVE FORWARD
// ---------------------------
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, LEFT_SPEED);
  analogWrite(ENB, RIGHT_SPEED);
}

// ---------------------------
// STOP MOTORS
// ---------------------------
void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 0);
}

// ---------------------------
// ROTATE 360° (non-blocking, fixed 1.35s)
// ---------------------------
void rotate360NonBlocking(unsigned long currentTime) {
  if (!rotationStarted) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENA, LEFT_SPEED);
    analogWrite(ENB, RIGHT_SPEED);
    rotateStartTime = currentTime;
    rotationStarted = true;
  }

  if (currentTime - rotateStartTime >= 1350) { // rotate for 1.35 sec
    stopMotors();
    state = STATE_DONE;
  }
}
