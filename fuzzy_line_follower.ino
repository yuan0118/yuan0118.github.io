#include <Arduino.h>

// Line sensor pins (3 sensors)
const int LEFT_SENSOR  = A0;
const int MID_SENSOR   = A1;
const int RIGHT_SENSOR = A2;

// Motor control pins
const int EN_LEFT  = 5;  // PWM pin for left motor
const int EN_RIGHT = 6;  // PWM pin for right motor
const int DIR_LEFT  = 7;  // direction pin for left motor
const int DIR_RIGHT = 8;  // direction pin for right motor

// Ultrasonic sensor for obstacle avoidance
const int TRIG_PIN = 9;
const int ECHO_PIN = 10;

// Base speed for both motors
const int BASE_SPEED = 150;  // 0-255

// Simple measurement of distance using HC-SR04
long measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // timeout 30 ms
  long distance = duration / 58; // convert to cm
  return distance;
}

// Read line sensors and calculate error (-2 .. 2)
int readLineError() {
  int l = digitalRead(LEFT_SENSOR);
  int m = digitalRead(MID_SENSOR);
  int r = digitalRead(RIGHT_SENSOR);

  // Assume sensors return LOW when seeing black line
  int error = 0;
  if (l == LOW && r == HIGH) error = -2;     // line on left
  else if (r == LOW && l == HIGH) error = 2; // line on right
  else if (m == LOW) {
    if (l == LOW && r == LOW) error = 0;     // centered
    else if (l == LOW) error = -1;
    else if (r == LOW) error = 1;
    else error = 0;
  }
  return error;
}

// Fuzzy logic controller: returns speed difference
int fuzzyControl(int error) {
  // Membership functions for Negative, Zero, Positive
  float negative = constrain(-error / 2.0, 0, 1);
  float positive = constrain(error / 2.0, 0, 1);
  float zero = 1.0 - max(negative, positive);

  // defuzzify to obtain speed difference (-100..100)
  float delta = negative * -100 + zero * 0 + positive * 100;
  return (int)delta;
}

// Set motor speeds (-255..255)
void setMotorSpeed(int leftSpeed, int rightSpeed) {
  if (leftSpeed >= 0) {
    digitalWrite(DIR_LEFT, HIGH);
  } else {
    digitalWrite(DIR_LEFT, LOW);
    leftSpeed = -leftSpeed;
  }
  if (rightSpeed >= 0) {
    digitalWrite(DIR_RIGHT, HIGH);
  } else {
    digitalWrite(DIR_RIGHT, LOW);
    rightSpeed = -rightSpeed;
  }
  analogWrite(EN_LEFT, constrain(leftSpeed, 0, 255));
  analogWrite(EN_RIGHT, constrain(rightSpeed, 0, 255));
}

void setup() {
  pinMode(LEFT_SENSOR, INPUT);
  pinMode(MID_SENSOR, INPUT);
  pinMode(RIGHT_SENSOR, INPUT);

  pinMode(EN_LEFT, OUTPUT);
  pinMode(EN_RIGHT, OUTPUT);
  pinMode(DIR_LEFT, OUTPUT);
  pinMode(DIR_RIGHT, OUTPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  // obstacle avoidance
  long dist = measureDistance();
  if (dist > 0 && dist < 15) {
    setMotorSpeed(0, 0); // stop if obstacle too close
    return;
  }

  int error = readLineError();
  int delta = fuzzyControl(error); // delta speed

  int leftSpeed = BASE_SPEED - delta;
  int rightSpeed = BASE_SPEED + delta;

  // Slow down when turning sharply
  if (abs(error) > 1) {
    leftSpeed /= 2;
    rightSpeed /= 2;
  }

  setMotorSpeed(leftSpeed, rightSpeed);
  delay(10);
}

