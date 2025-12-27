// MPU6050 Pitch Angle Measurement and State-Based Control
// Used to detect ramp climbing using accelerometer data

#include <Wire.h>

const int MPU = 0x68;

// Pitch angle (degrees)
float pitchAngle = 0;

// State definitions
#define STATE_CLIMB  0
#define STATE_ROTATE 1

int state = STATE_CLIMB;

void setup() {
  Wire.begin();

  // Wake up MPU6050
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
}

void loop() {
  readMPU();

  // Ramp climbing detected
  if (state == STATE_CLIMB && pitchAngle > 25) {
    stopMotors();
    delay(4000);          // Stabilise at top of ramp
    state = STATE_ROTATE;
  }

  if (state == STATE_ROTATE) {
    rotateInPlace();
  }
}

// Reads accelerometer data and calculates pitch angle
void readMPU() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // Accelerometer data register
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);

  int16_t AccX = Wire.read() << 8 | Wire.read();
  int16_t AccY = Wire.read() << 8 | Wire.read();
  int16_t AccZ = Wire.read() << 8 | Wire.read();

  float Ax = AccX / 16384.0;
  float Ay = AccY / 16384.0;
  float Az = AccZ / 16384.0;

  pitchAngle = -atan2(Ax, sqrt(Ay * Ay + Az * Az)) * 180 / PI;
}

// Placeholder motor control functions
void stopMotors() {
  // Motor stop logic
}

void rotateInPlace() {
  // Rotation logic
}
