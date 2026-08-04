/***
* Shyam Kishan
* sentinel_v3.ino
* Implemented a reactive obstacle-avoidance system with directional sampling and decision logic.
* This implementation has both manual and automatic modes, where the manual mode can be controlled from a web app.
***/

#include <Servo.h>
#include <stdio.h>
#include <stdbool.h>

// RIGHT Motor Pins
const int ENA = 5;      // this value directly changes speed of 2 right wheels (0 to 255)
const int IN1 = 7;      // this value changes the direction of 2 right wheels spinning (HIGH == FORWARD, LOW == BACKWARD)

// LEFT Motor Pins
const int ENB = 6;      // this value directly changes the speed of 2 left wheels (0 to 255)
const int IN2 = 8;      // this value changes the direction of 2 left wheels spinning (HIGH == FORWARD, LOW == BACKWARD)

// Ultronsonic Sensor Pins
const int TRIG = 13;
const int ECHO = 12;
const int OBSTACLE_DISTANCE = 40;

// Servo Motor Pin
const int servoPin = 11;
Servo myServo;
int pos = 0;

// Constants for turning Sentinel
float LEFT_TURN_TIME = 1746.875;
float RIGHT_TURN_TIME = 1900;

// Time Tracking Variables
unsigned long current_time;
unsigned long lastPrintTime, lastServoMoveTime, lastScanTime, rightTurnTime, leftTurnTime, lastDecisionTime = 0;

int angle = 90;
int step = 30;
int leftDist, rightDist, frontDist;
bool turning = false;


bool check_time(unsigned long &prevTime, const unsigned long interval) {
  if (current_time - prevTime >= interval) {
    prevTime = current_time;
    return true;
  }
  return false;
}

// drive_forward()
// Makes robot drive forward at a specifc speed, given by parameter `speed`.
void drive_forward(int speed) {
  // RIGHT Motors
  digitalWrite(IN1, HIGH);
  analogWrite(ENA, speed); // Speed: 0 (Off) to 255 (Max)

  // LEFT Motors
  digitalWrite(IN2, HIGH);
  analogWrite(ENB, speed); // Speed: 0 (Off) to 255 (Max)
}

// drive_backward()
// Makes robot drive backward at a specific speed, given by parameter `speed`.
void drive_backward(int speed) {
  // RIGHT Motors
  digitalWrite(IN1, LOW);
  analogWrite(ENA, speed); // Speed: 0 (Off) to 255 (Max)

  // LEFT Motors
  digitalWrite(IN2, LOW);
  analogWrite(ENB, speed); // Speed: 0 (Off) to 255 (Max)
}

// turn_right()
// Makes robot turn right at the parameter `deg` degrees.
void turn_right(int deg) {
  // RIGHT Motors
  digitalWrite(IN1, LOW);
  analogWrite(ENA, 150);        // Speed: 0 (Off) to 255 (Max)

  // LEFT Motors
  digitalWrite(IN2, HIGH);
  analogWrite(ENB, 150);        // Speed: 0 (Off) to 255 (Max)

  float time_to_delay = (RIGHT_TURN_TIME / 360.0) * deg;
  if (check_time(rightTurnTime, time_to_delay)) {
    stop();
  }

}

// turn_left()
// Makes robot turn left at the parameter `deg` degrees.
void turn_left(int deg) {
  // RIGHT Motors
  digitalWrite(IN1, HIGH);
  analogWrite(ENA, 150);        // Speed: 0 (Off) to 255 (Max)

  // LEFT Motors
  digitalWrite(IN2, LOW);
  analogWrite(ENB, 150); // Speed: 0 (Off) to 255 (Max)
  
  float time_to_delay = (LEFT_TURN_TIME / 360.0) * deg;
  if (check_time(leftTurnTime, time_to_delay)) {
    stop();
  }  
}

// stop()
// Makes robot stop moving completely.
void stop() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

// getDistance()
// Uses the Ultrasonic Sensor to measure the distance between the robot and an obstacle in front of it.
// returns the distance measured in cm.
float getDistance() {
  float duration, distance;
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);

  duration = pulseIn(ECHO, HIGH, 30000);
  distance = (duration * 0.0343) / 2;
  return distance;
}


// getStableDistance()
// Uses the getDistance() method multiple times within a specific time to filter garbage values read from the Ultrasonic sensor.
// returns the average distance measured in cm.
float getStableDistance(){
  float d = getDistance();
  if (d > 2 && d < 300) {
    return d;
  }

  return 999;
}

void drive_arc(int leftSpeed, int rightSpeed){
  // RIGHT Motors
  digitalWrite(IN1, HIGH);
  analogWrite(ENA, leftSpeed); // Speed: 0 (Off) to 255 (Max)

  // LEFT Motors
  digitalWrite(IN2, HIGH);
  analogWrite(ENB, rightSpeed); // Speed: 0 (Off) to 255 (Max)
}

float* updateScanRecord(float distances[19], float recordedDistance) {
  if (angle >= 0 && angle <= 180) {
    int distanceIndex = angle / 10;
    distances[distanceIndex] = recordedDistance;
    return distances;
  }
}

void setup() {
  // Set all motor control pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(3, OUTPUT);

  // Set Ultrasonic Sensor Pins accordingly
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  // Set Servo Motor accordingly
  myServo.attach(servoPin);
  myServo.write(90);

  Serial.begin(115200);
}

float frontEMA = getStableDistance();
float leftEMA = frontEMA;
float rightEMA = frontEMA;
float alpha = 0.9;
int dangerCount = 0;
bool leftReady, rightReady, frontReady = false;
bool auto_mode = false;
String cmd = "";
float distance = 0;

// autoMode()
// Sentinel follows this algorithm to avoid obstacles that are in its path.
void autoMode () {
  if (!auto_mode) {
    return;
  }
  
  int speed = 100;
  if (frontEMA < OBSTACLE_DISTANCE && !turning) {
    // Serial.println("Driving Backward");
    drive_backward(50);
  } else if (leftEMA < OBSTACLE_DISTANCE && rightEMA > leftEMA) {
      // Serial.println("Turning Right");
      turning = true;
      drive_arc(100, 0);
  } else if (rightEMA < OBSTACLE_DISTANCE && leftEMA > rightEMA) {
      // Serial.println("Turning Left");
      turning = true;
      drive_arc(0, 100);
  } else {
      // Serial.println("Driving Forward");
      turning = false;
      drive_forward(speed);
  }
}

// manualMode()
// takes paramater `cmd` and moves the robot in the direction/mode `cmd` tells it.
void manualMode (String cmd) {
  if      (cmd == "forward") {
    // Serial.println("Manual forward");
    drive_forward(150);
  }
  else if (cmd == "left") {
    // Serial.println("Manual left");
    drive_arc(200, 0);
  }
  else if (cmd == "right") {
    // Serial.println("Manual right");
    drive_arc(0, 200);
  }
  else if (cmd == "stop") {
    // Serial.println("Manual stop");
    stop();
  }
  else if (cmd == "backward") {
    // Serial.println("Manual backward");
    drive_backward(150);
  }
}

void loop(){
  digitalWrite(3, HIGH);
  current_time = millis();

  // Check to see if Python Script gave any input to Arduino
  if (Serial.available()) {
    cmd = Serial.readStringUntil('\n');
    if (cmd == "auto") auto_mode = true;
    if (cmd == "manual") {
      auto_mode = false;
      stop();
    }
  }

  if (auto_mode) {
    autoMode();
    // makes the Servo move from 0º to 180º
    if (check_time(lastServoMoveTime, 150)) {
        angle += step;
        if (angle >= 180 || angle <= 0) step = -step;
        myServo.write(angle);
    }
  } else {
    manualMode(cmd);
    myServo.write(90);
  }

  if (check_time(lastScanTime, 50)) {
    distance = getStableDistance();
  }

  if (angle <= 70) {
    leftDist = distance;
    leftEMA  = alpha * leftDist  + (1 - alpha) * leftEMA;
    leftReady = true;
  } else if (angle >= 110) {
    rightDist = distance;
    rightEMA = alpha * rightDist + (1 - alpha) * rightEMA;
    rightReady = true;
  } else {
    frontDist = distance;
    frontEMA = alpha * frontDist + (1 - alpha) * frontEMA;
    frontReady = true;
  }

  if (check_time(lastPrintTime, 100)) {
    Serial.write(0xAA);
    Serial.write(0x55);
    Serial.write((uint8_t*)&distance, 4);

  }
}
