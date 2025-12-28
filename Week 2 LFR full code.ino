#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ---------------- LCD ----------------
LiquidCrystal_I2C lcd(0x27, 16, 2); 
unsigned long startTime;

// ---------------- MOTOR PINS ----------------
const int ENA = A1, IN1 = 12, IN2 = 13;   // Left motor
const int ENB = 3,  IN3 = 2, IN4 = 1;    // Right motor

// ---------------- SENSORS -------------------
const int leftSensor  = A2;
const int rightSensor = A3;

// ---------------- SETTINGS ------------------
int threshold   = 400;   // Adjust using Serial Monitor
int leftSpeed   = 150;
int rightSpeed  = 205;

// ---------------- SETUP ---------------------
void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(leftSensor, INPUT);
  pinMode(rightSensor, INPUT);

  Serial.begin(9600);

  // LCD setup
  lcd.init();
  lcd.backlight();
  startTime = millis();
  lcd.setCursor(0,0);
  lcd.print("Line Follower");
  delay(1000);
}

// ---------------- MAIN LOOP ------------------
void loop() {
  int L = analogRead(leftSensor);
  int R = analogRead(rightSensor);

  Serial.print("L: ");
  Serial.print(L);
  Serial.print(" | R: ");
  Serial.println(R);

  // ---- LCD Display ----
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("L:");
  lcd.print(L);
  lcd.print(" R:");
  lcd.print(R);

  lcd.setCursor(0,1);
  lcd.print("T:");
  lcd.print((millis()-startTime)/1000);
  lcd.print("s");

  // ---- Line Logic ----
  bool leftOnLine  = (L < threshold);   
  bool rightOnLine = (R < threshold);

  if (leftOnLine && rightOnLine) {
    moveForward();
  }
  else if (!leftOnLine && rightOnLine) {
    turnRightSoft();
  }
  else if (leftOnLine && !rightOnLine) {
    turnLeftSoft();
  }
  else {
    searchLine();
  }
}

// ---------------- MOTOR FUNCTIONS ----------------

void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, leftSpeed);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, rightSpeed);
}

void turnLeftSoft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 120);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 180);
}

void turnRightSoft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 180);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 120);
}

void searchLine() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 120);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 120);
}
