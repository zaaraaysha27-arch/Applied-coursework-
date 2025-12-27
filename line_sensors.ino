// Line Sensor Processing and Line-Following Logic
// Uses two analogue sensors for differential line detection

// ----- Sensor Inputs -----
const int leftSensor  = A2;
const int rightSensor = A3;

// Threshold value (tuned experimentally)
int threshold = 400;

void setup() {
  pinMode(leftSensor, INPUT);
  pinMode(rightSensor, INPUT);
}

void loop() {
  int L = analogRead(leftSensor);
  int R = analogRead(rightSensor);

  bool leftOnLine  = (L < threshold);
  bool rightOnLine = (R < threshold);

  if (leftOnLine && rightOnLine) {
    moveForward();
  }
  else if (leftOnLine && !rightOnLine) {
    turnLeftSoft();
  }
  else if (!leftOnLine && rightOnLine) {
    turnRightSoft();
  }
  else {
    searchLine();   // Recovery behaviour
  }
}

// Placeholder motor control functions
void moveForward() {
  // Forward motion logic
}

void turnLeftSoft() {
  // Soft left turn logic
}

void turnRightSoft() {
  // Soft right turn logic
}

void searchLine() {
  // Line reacquisition logic
}
