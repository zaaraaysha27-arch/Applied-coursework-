// Rotary Encoder Distance Measurement
// Used for measuring wheel travel using pulse counting

const int encoderPin = A0;
long pulseCount = 0;
int lastState = HIGH;

// Encoder & wheel parameters
const int pulsesPerRev = 20;          // adjust based on encoder
const float wheelCircumference = 0.21; // metres (example)
const float targetDistance = 1.0;      // metres

void setup() {
  pinMode(encoderPin, INPUT);
}

void loop() {
  int currentState = digitalRead(encoderPin);

  // Detect falling edge
  if (lastState == HIGH && currentState == LOW) {
    pulseCount++;
    delay(2);   // debounce delay
  }

  lastState = currentState;

  float distance = (pulseCount / (float)pulsesPerRev) * wheelCircumference;

  if (distance >= targetDistance) {
    stopMotors();
  }
}

// Stops the motors once target distance is reached
void stopMotors() {
  // Motor stop logic here
}
