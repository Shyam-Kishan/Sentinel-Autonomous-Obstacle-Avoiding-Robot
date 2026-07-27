/***
* Shyam Kishan
* sentinel_v2.ino
* Implemented a reactive obstacle-avoidance system with directional sampling and decision logic
***/

#include <Servo.h>

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
  delay((int)time_to_delay);
  stop();
  delay(100);
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
  delay((int)time_to_delay);
  stop();
  delay(100);
}

// stop()
// Makes robot stop moving completely.
void stop() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

// left_90_deg()
// Makes robot turn 90º left
void left_90_deg(){
  // RIGHT Motors
  digitalWrite(IN1, HIGH);
  analogWrite(ENA, 255);

  // LEFT Motors
  digitalWrite(IN2, LOW);
  analogWrite(ENB, 255);
  delay(233);
  stop();
}

// right_90_deg()
// Makes robot turn 90º right
void right_90_deg(){
  // RIGHT Motors
  digitalWrite(IN1, LOW);
  analogWrite(ENA, 255);

  // LEFT Motors
  digitalWrite(IN2, HIGH);
  analogWrite(ENB, 255);
  delay(233);
  stop();
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
// scanAtAngle()
// uses the getStableDistance() to measure the distance between the Ultrasonic sensor and the nearest obstacle is at `angle` degrees from robot.
// returns distance in cm.
float scanAtAngle(int angle){
  myServo.write(angle);
  delay(250);
  float d = getStableDistance();
  delay(50);
  return d;
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

void loop(){
  digitalWrite(3, HIGH); 
  float distance = getStableDistance();
  Serial.print("Distance: ");
  Serial.println(distance);
  delay(50);

  int speed;
  if (distance > 100) {
    speed = 150;
  } else if (distance > 60) {
    speed = 130;
  } else if (distance > 40) {
    speed = 110;
  } else {
    stop();
    float distances[5];
    int angles[5] = {0, 45, 90, 135, 180};
    
    for (int i = 0; i < 5; i++) {
      distances[i] = scanAtAngle(angles[i]);
    }

    // Look Center
    myServo.write(90);
    delay(150);

    int bestIndex = -1;
    float bestDist = 0;

    for (int i = 0; i < 5; i++) {
      if (distances[i] > OBSTACLE_DISTANCE && distances[i] > bestDist){
        Serial.print("Distance ");
        Serial.print(i);
        Serial.println(distances[i]);
        bestDist = distances[i];
        bestIndex = i;
      }
    }

    if (bestIndex == -1) {
      // No good path, back up
      drive_backward(120);
      delay(1000);
      stop();
      return;
      
    }
    // best path is right 90º
    else if (bestIndex == 0) {
      turn_right(90);

    // best path is right 45º
    } else if (bestIndex == 1) {
      turn_right(45);

    // best path is left 45º
    } else if (bestIndex == 3) {
      turn_left(45);

    // best path is left 90º
    } else if (bestIndex == 4) {
      turn_left(90);

    // best path is forward
    } else if (bestIndex == 2){
      return;
    }
    delay(150);
    return;
  }
  drive_forward(speed);
}
