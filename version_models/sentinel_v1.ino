/***
* Shyam Kishan
* sentinel_v1.ino
* Implementation of basic controls for robotic car 7/25/26
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
// Makes robot turn right slightly while driving forward.
// Robot's speed is initalized by parameter `speed`.
void turn_right(int speed) {
  int slowed_speed = speed - 40;

  if (slowed_speed < 0) {
    slowed_speed = 0;
  }

  // RIGHT Motors
  digitalWrite(IN1, HIGH);
  analogWrite(ENA, slowed_speed); // Speed: 0 (Off) to 255 (Max)

  // LEFT Motors
  digitalWrite(IN2, HIGH);
  analogWrite(ENB, speed);        // Speed: 0 (Off) to 255 (Max)
}

// turn_left()
// Makes robot turn left slightly while driving forward.
// Robot's speed is initalized by parameter `speed`.
void turn_left(int speed) {
  int slowed_speed = speed - 40;

  if (slowed_speed < 0) {
    slowed_speed = 0;
  }
  // RIGHT Motors
  digitalWrite(IN1, HIGH);
  analogWrite(ENA, speed);        // Speed: 0 (Off) to 255 (Max)

  // LEFT Motors
  digitalWrite(IN2, HIGH);
  analogWrite(ENB, slowed_speed); // Speed: 0 (Off) to 255 (Max)
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

  duration = pulseIn(ECHO, HIGH);
  distance = (duration * 0.0343) / 2;
  return distance;
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
  // This code lets robot drive forward until it is 40 cm or less from an obstacle in front of it.
  // When it is 40 cm from an obstacle or less, the robot checks the furthest obstacle from its left and right.
  // The robot then makes a decision to turn in whichever direction has the furthest obstacle. 
  digitalWrite(3, HIGH);

  float distance = getDistance();
  delay(50);

  if (distance > OBSTACLE_DISTANCE) {
    drive_forward(100);
  } else {
      float left_dist, right_dist;

      stop();
      delay(300);

      // Look Left
      myServo.write(180);           // turn Servo LEFT 90º from forward position
      delay(300);
      left_dist = getDistance();    // capture distance of nearest obstacle from LEFT side

      // Look Right
      myServo.write(0);             // turn Servo RIGHT 90º from forward position
      delay(1000);
      right_dist = getDistance();   // capture distance of nearest obstacle from RIGHT side
      
      // Look Center
      myServo.write(90);            // reset Servo back to forward position
      delay(200);

    if (left_dist > right_dist) {
      left_90_deg();            
    } else {
      right_90_deg();
    }
  }
}
