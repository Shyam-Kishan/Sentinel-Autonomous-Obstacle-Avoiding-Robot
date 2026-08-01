/***
* Shyam Kishan
* sentinel_v2.5.ino
* Implemented a reactive obstacle-avoidance system with directional sampling and decision logic.
* This implementation works the same as v2, but no/minimal usage of the `delay()` method.
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
unsigned long lastPrintTime, lastScanTime1, lastScanTime2, lastServoMoveTime, lastTurningTime = 0;
unsigned long lastStopTime, leftTurnTime, rightTurnTime = 0;

bool check_time(unsigned long &prevTime, const unsigned long interval) {
  if (current_time - prevTime >= interval) {
    prevTime = current_time;
    return true;
  }
  return false;
}
const unsigned long tenMS = 10;
const unsigned long fiftyMS = 50;
const unsigned long oneHundredMS = 100;
const unsigned long oneHundredFiftyMS = 150;
const unsigned long twoHundredFiftyMS = 250;
const unsigned long oneSec = 1000;


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

  /*
  float time_to_delay = (RIGHT_TURN_TIME / 360.0) * deg;
  if (check_time(rightTurnTime, time_to_delay)) {
    stop();
  }
  */
  /*
  delay((int)time_to_delay);
  stop();
  delay(100);
  */
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
  /*
  float time_to_delay = (LEFT_TURN_TIME / 360.0) * deg;
  if (check_time(leftTurnTime, time_to_delay)) {
    stop();
  }
  */
  /*
  delay((int)time_to_delay);
  stop();
  */
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
  float total = 0;
  int count = 0;

  for (int i = 0; i < 5; i++) {
    float d = getDistance();
    if (d > 2 && d < 300) {     // filtering garbage values
      total += d;
      count++;
    }
    delay(10);
  }
  if (count == 0) return 999;   // no valid readings
  return total / count;
}

enum ScanState {
  MOVE_SERVO,
  WAIT_FOR_SERVO,
  READ_DISTANCE,
  DONE
};

ScanState scanState = MOVE_SERVO;
unsigned long scanStartTime;
float scanDistance;
int currentAngle;

// scanAtAngle()
// uses the getStableDistance() to measure the distance between the Ultrasonic sensor and the nearest obstacle is at `angle` degrees from robot.
// returns distance in cm.
float scanAtAngle(int angle){
  switch(scanState) {
    case MOVE_SERVO:
      Serial.println("MOVE_SERVO");
      currentAngle = angle;
      Serial.print("Current Angle: ");
      Serial.println(currentAngle);
      myServo.write(currentAngle);
      scanStartTime = current_time;
      scanState = WAIT_FOR_SERVO;
      break;
    case WAIT_FOR_SERVO:
      Serial.println("WAITING FOR SERVO");
      if (check_time(scanStartTime, twoHundredFiftyMS)) {
        scanState = READ_DISTANCE;
      }
      break;
    case READ_DISTANCE:
      Serial.println("READING DISTANCE");
      scanDistance = getStableDistance();
      scanState = DONE;
      break;
    case DONE:
      Serial.println("DONE WITH DISTANCE");
      myServo.write(90);
      scanState = MOVE_SERVO;
      return scanDistance;
      break;
  }
  return -1;
}

enum AvoidState {
  START_SCAN,
  SCANNING,
  DECIDE,
  ACT, 
  TURNING,
  DRIVING
};

AvoidState avoidState = START_SCAN;
int scanIndex = 0;
int bestIndex = -1;
float distances[5];
int angles[5] = {0, 45, 90, 135, 180};

int pendingTurnDir = 0;   // -1 = left, 1 = right, 0 = none
int pendingTurnDeg = 0;
unsigned long turnStartTime = 0;

void startTurn(int dir, int deg) {
  pendingTurnDir = dir;
  pendingTurnDeg = deg;
  turnStartTime = current_time;
  if (dir == 1) {
    // RIGHT Motors
    digitalWrite(IN1, LOW);
    analogWrite(ENA, 150);        // Speed: 0 (Off) to 255 (Max)

    // LEFT Motors
    digitalWrite(IN2, HIGH);
    analogWrite(ENB, 150);        // Speed: 0 (Off) to 255 (Max)
  } else {
    // RIGHT Motors
    digitalWrite(IN1, HIGH);
    analogWrite(ENA, 150);        // Speed: 0 (Off) to 255 (Max)

    // LEFT Motors
    digitalWrite(IN2, LOW);
    analogWrite(ENB, 150); // Speed: 0 (Off) to 255 (Max)
  }
}

bool turnInProgress() {
  float turnTime = ((pendingTurnDir == 1 ? RIGHT_TURN_TIME: LEFT_TURN_TIME) / 360.0) * pendingTurnDeg;
  if (current_time - turnStartTime >= turnTime) {
    stop();
    return false;
  }
  return true;
}

void avoidObstacle() {
  // Serial.print("STATE: ");
  // Serial.println(avoidState);
  switch(avoidState) {
    case START_SCAN:
      Serial.println("STARTING SCAN");
      if (check_time(lastScanTime1, oneSec)) {
        scanIndex = 0;
        avoidState = SCANNING;
      }
      break;
    case SCANNING:
      Serial.println("SCANNING");
      if (scanIndex < 5) {
        float result = scanAtAngle(angles[scanIndex]);
        if (result != -1) {
          distances[scanIndex] = result;
          scanIndex++;
        }

        Serial.print("scanIndex: ");
        Serial.println(scanIndex);

        if (scanIndex >= 5) {
          avoidState = DECIDE;
        }
      }
      break;
      
   case DECIDE: {
      Serial.println("DECIDING");
      delay(200);
      bestIndex = -1;
      float bestDist = 0;

      for (int i = 0; i < 5; i++) {
        if (distances[i] > OBSTACLE_DISTANCE && distances[i] > bestDist){
          bestDist = distances[i];
          bestIndex = i;
        }
      }
      avoidState = ACT;

      Serial.print("Best Distance: ");
      Serial.println(bestDist);

      Serial.print("Best Index: ");
      Serial.println(bestIndex);
      break;
   }
    case ACT:
      Serial.println("ACTING");
      if (bestIndex == -1) {
        // No good path, back up
        Serial.println("Going Backward");
        drive_backward(120);
        delay(1000);
        stop();
        avoidState = TURNING;
        break;
      // best path is right 90º
      } else if (bestIndex == 0) {
        Serial.println("Turning right 90 deg");
        // turn_right(90);
        startTurn(1, 90);
        avoidState = TURNING;
        break;

      // best path is right 45º
      } else if (bestIndex == 1) {
        Serial.println("Turning right 45 deg");
        // turn_right(45);
        startTurn(1, 45);
        avoidState = TURNING;
        break;

      // best path is left 45º
      } else if (bestIndex == 3) {
        Serial.println("Turning left 45 deg");
        // turn_left(45);
        startTurn(-1, 45);
        avoidState = TURNING;
        break;

      // best path is left 90º
      } else if (bestIndex == 4) {
        Serial.println("Turning left 90 deg");
        // turn_left(90);
        startTurn(-1, 90);
        avoidState = TURNING;
        break;

      // best path is forward
      } else if (bestIndex == 2){
        Serial.println("Going Forward");
        avoidState = START_SCAN;
        break;
      }
    case TURNING: 
      if (!turnInProgress()) {
        avoidState = START_SCAN;
      }
      break;
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
  Serial.begin(9600);
}

bool avoidanceActive = false;

void loop(){
  digitalWrite(3, HIGH);
  current_time = millis();
  float distance;
  if (!avoidanceActive) {
    if (check_time(lastPrintTime, fiftyMS)) {
      distance = getStableDistance();
      Serial.print("Distance: ");
      Serial.println(distance);
    }

    if (distance > OBSTACLE_DISTANCE) {
      drive_forward(130);
    } else {
      avoidanceActive = true;
      stop();
    }  
  } else {
    avoidObstacle();
    if (avoidState == START_SCAN) {
      avoidanceActive = false;
    }
  }
}
