//Servos
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN  90 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)
int s1Pos = 155;
int s2Pos = 110;
int s3Pos = 107;
int s4Pos = 20;
int s5Pos = 50;//80 closed
int s6Pos = 110;//80 closed

//Motor driver
#include "CytronMD.h"
// Configure the motor driver.
CytronMD motor1(13, 12);
CytronMD motor2(14, 27);
CytronMD motor3(26, 25);
CytronMD motor4(33, 32);

//Base ultrasonic
#include "Ultrasonic.h"
Ultrasonic ultrasonic1(2);//f1
Ultrasonic ultrasonic2(15);//f2
Ultrasonic ultrasonic3(4);//f3
Ultrasonic ultrasonic4(5);//f4
//sensor readings
int f1,f2,f3,f4;
//Other ultrasonics
const int trigPin1 = 16;
const int echoPin1 = 17;
//right
const int trigPin2 = 23;
const int echoPin2 = 35;
//left
const int trigPin3 = 18;
const int echoPin3 = 19;
//define sound speed in cm/uS
#define SOUND_SPEED 0.034
//sensor readings
float leftS,rightS,frontS; 

//Metal detector
#define metalSensorPin 34
int metalSensorValue;
bool isMetal = false;

struct ServoPositions {
  int finalPos1;
  int finalPos2;
  int finalPos3;
};


void setup() {
  Serial.begin(115200);
  Serial.println("BOA Starting..");
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  //Ultrasonic
  pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin2, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin3, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin3, INPUT); // Sets the echoPin as an Input
  //Default
  pwm.setPWM(0, 0, angleToPulse(s1Pos));//+left
  pwm.setPWM(1, 0, angleToPulse(s2Pos)); //+up
  pwm.setPWM(2, 0, angleToPulse(s3Pos));//-up
  pwm.setPWM(3, 0, angleToPulse(s4Pos)); //+up
  //Gripper
  pwm.setPWM(4, 0, angleToPulse(s5Pos)); //- open
  pwm.setPWM(5, 0, angleToPulse(s6Pos)); //+ open
}



////////////////////////////////////////

bool isForward = false;
bool isLeft = false;
bool isRight = false;

enum RobotState {
  SCAN_LEFT_TO_RIGHT,
  SCAN_RIGHT_TO_LEFT,
  MOVE_FORWARD,
  STOP
};

RobotState currentState = SCAN_LEFT_TO_RIGHT;
RobotState lastState;
unsigned long lastStateChangeTime = 0;
unsigned long forwardDuration = 3000; // Time to move forward in milliseconds


void loop() {
  // Read ultrasonic sensors
  readUltrasonicSeedStudio();
  readNormalUltrasonic();
  readMetalSensor();

    switch (currentState) {
      case SCAN_LEFT_TO_RIGHT:
        scanLeftToRight();
        break;
      case SCAN_RIGHT_TO_LEFT:
        scanRightToLeft();
        break;
      case MOVE_FORWARD:
        moveForwardForDuration();
        break;
      case STOP:
        stopMovement();
        break;
  }
}



void scanLeftToRight() {
  lastState = SCAN_LEFT_TO_RIGHT;
  moveRightSide();

  takeWaste();

  // Check if the robot has scanned the room
  if (isAtRightmostEnd()) {
    currentState = MOVE_FORWARD;
    lastStateChangeTime = millis(); // Record the time when the state changed
  }
}

void scanRightToLeft() {
  lastState = SCAN_RIGHT_TO_LEFT;
  moveLeftSide();

  takeWaste();

  // Check if the robot has scanned the room
  if (isAtLeftmostEnd()) {
    currentState = MOVE_FORWARD;
    lastStateChangeTime = millis(); // Record the time when the state changed
  }
}


void moveForwardForDuration() {
  // Move forward for a set duration
  if (millis() - lastStateChangeTime < forwardDuration) {

    // Serial.println(frontS);

    if(frontS > 30 || frontS == 0) {
      moveForward();
    } else {
      currentState = STOP;
    }
    
  } else {
    if(lastState == SCAN_LEFT_TO_RIGHT)
      currentState = SCAN_RIGHT_TO_LEFT; // Switch back to scanning left to right
    else if(lastState == SCAN_RIGHT_TO_LEFT)
      currentState = SCAN_LEFT_TO_RIGHT;
    // Reset any other necessary variables
  }
}


bool isAtLeftmostEnd() {
  //Implement logic to determine if the robot is at the rightmost end of the room
  return leftS < 30;
}

bool isAtRightmostEnd() {
  //Implement logic to determine if the robot is at the rightmost end of the room
  return rightS < 30;
}

void takeWaste(){
  if(f1 < 12) {
      delay(1000);
      currentState = STOP;
      stopMovement();
      pickWaste(false, true);
      readMetalSensor();
      delay(1000);
      dropWaste();
      currentState = lastState;
  } else if (f2 < 12){
      currentState = STOP;
      stopMovement();
      pickWaste(false, true);
      readMetalSensor();
      delay(1000);
      dropWaste();
      currentState = lastState;
  } else if(f3 < 12) {
      currentState = STOP;
      stopMovement();
      pickWaste(true, false);
      readMetalSensor();
      delay(1000);
      dropWaste();
      currentState = lastState;
  } else if(f4 < 12) {
      delay(1000);
      currentState = STOP;
      stopMovement();
      pickWaste(true, false);
      readMetalSensor();
      delay(1000);
      dropWaste();
      currentState = lastState;
  } 
}


//////////////////////////////////////////


void pickWaste(bool isLeft, bool isRight){
  s1Pos = moveServosSlowly(1, s1Pos, isRight ? 130 : isLeft ? 170 : 155, 20); // Servo 3

  // s2Pos = moveServosSlowly(2, s2Pos, 120, 50); // Servo 2
  // s3Pos = moveServosSlowly(3, s3Pos, 160, 50); // Servo 3

  ServoPositions finalPositions = moveThreeServosSlowly(2, s2Pos, 3, s3Pos, 4, s4Pos, isLeft || isRight ? 70 : 80, isLeft || isRight ? 100 :  120, 10, 50);
  s2Pos = finalPositions.finalPos1;
  s3Pos = finalPositions.finalPos2;
  s4Pos = finalPositions.finalPos3;

  // s4Pos = moveServosSlowly(4, s4Pos, 0, 50); // Servo 4
  // s6Pos = moveServosSlowly(6, s6Pos, 80, 50); // Servo 6
  // s5Pos = moveServosSlowly(5, s5Pos, 80, 50); // Servo 5
  //--------------------------------
  ServoPositions finalPositions2 = moveThreeServosSlowly(5, s5Pos, 6, s6Pos, 3, s3Pos, 85, 75, 130, 50);
  s5Pos = finalPositions2.finalPos1;
  s6Pos = finalPositions2.finalPos2;
  s3Pos = finalPositions2.finalPos3;
  // Serial.println(s5Pos);
  // Serial.println(s6Pos);
  // Serial.println(s3Pos);
}

void dropWaste(){
  if(isMetal){
    isMetal = false;
    dropMetalWaste();
  } else {
    dropNormalWaste();
  }
}

void dropMetalWaste(){
  s3Pos = moveServosSlowly(3, s3Pos, 100, 50); // Servo 3
  s2Pos = moveServosSlowly(2, s2Pos, 100, 50); // Servo 2
  s4Pos = moveServosSlowly(4, s4Pos, 0, 50); // Servo 4
  s1Pos = moveServosSlowly(1, s1Pos, 0, 30); // Servo 3
  // s6Pos = moveServosSlowly(6, s6Pos, 80, 50); // Servo 6
  // s5Pos = moveServosSlowly(5, s5Pos, 80, 50); // Servo 5
  ServoPositions finalPositions = moveThreeServosSlowly(5, s5Pos, 6, s6Pos, 3, s3Pos, 50, 110, 110, 50);
  s5Pos = finalPositions.finalPos1;
  s6Pos = finalPositions.finalPos2;
  s3Pos = finalPositions.finalPos3;
  s3Pos = moveServosSlowly(3, s3Pos, 80, 50); // Servo 3
}

void dropNormalWaste(){
  s2Pos = moveServosSlowly(2, s2Pos, 170, 50); // Servo 2
  s4Pos = moveServosSlowly(4, s4Pos, 0, 50); // Servo 4
  s1Pos = moveServosSlowly(1, s1Pos, 145, 30); // Servo 3

  s3Pos = moveServosSlowly(3, s3Pos, 70, 10); // Servo 3


  // s6Pos = moveServosSlowly(6, s6Pos, 80, 50); // Servo 6
  // s5Pos = moveServosSlowly(5, s5Pos, 80, 50); // Servo 5
  ServoPositions finalPositions = moveThreeServosSlowly(5, s5Pos, 6, s6Pos, 3, s3Pos, 50, 110, 110, 50);
  s5Pos = finalPositions.finalPos1;
  s6Pos = finalPositions.finalPos2;
  s3Pos = finalPositions.finalPos3;
  // s3Pos = moveServosSlowly(3, s3Pos, 80, 50); // Servo 3
}

int moveServosSlowly(int servoNum, int startPos, int endPos, int stepDelay) {
  int currentPos = startPos;
  int increment = (endPos > startPos) ? 1 : -1; // Determine whether to increment or decrement
  int servoNumUpdated = servoNum - 1;
  
  // Loop until the servo reaches the target position
  while (currentPos != endPos) {
    currentPos += increment; // Update the current position
    // Limit the increment to avoid overshooting the target position
    if ((increment == 1 && currentPos > endPos) || (increment == -1 && currentPos < endPos)) {
      currentPos = endPos;
    }
    pwm.setPWM(servoNumUpdated, 0, angleToPulse(currentPos)); // Set the servo position
    delay(stepDelay); // Delay for smooth movement
  }
  return currentPos;
}


ServoPositions moveThreeServosSlowly(int servoNum1, int startPos1, int servoNum2, int startPos2, int servoNum3, int startPos3, int finalAngle1, int finalAngle2, int finalAngle3, int stepDelay) {
  int currentPos1 = startPos1;
  int currentPos2 = startPos2;
  int currentPos3 = startPos3;
  
  // Determine whether to increment or decrement for each servo
  int increment1 = (finalAngle1 > startPos1) ? 1 : -1;
  int increment2 = (finalAngle2 > startPos2) ? 1 : -1;
  int increment3 = (finalAngle3 > startPos3) ? 1 : -1;

  // Loop until all three servos reach their final angles
  while (currentPos1 != finalAngle1 || currentPos2 != finalAngle2 || currentPos3 != finalAngle3) {
    // Update the current positions for all three servos
    if (currentPos1 != finalAngle1) {
      currentPos1 += increment1;
    }
    if (currentPos2 != finalAngle2) {
      currentPos2 += increment2;
    }
    if (currentPos3 != finalAngle3) {
      currentPos3 += increment3;
    }

    // Set the positions for all three servos
    pwm.setPWM(servoNum1 - 1, 0, angleToPulse(currentPos1));
    pwm.setPWM(servoNum2 - 1, 0, angleToPulse(currentPos2));
    pwm.setPWM(servoNum3 - 1, 0, angleToPulse(currentPos3));

    delay(stepDelay); // Delay for smooth movement
  }
  
  // Create a ServoPositions structure and assign final positions
  ServoPositions finalPositions;
  finalPositions.finalPos1 = currentPos1;
  finalPositions.finalPos2 = currentPos2;
  finalPositions.finalPos3 = currentPos3;
  
  return finalPositions;
}

//------------------------------------------

void readMetalSensor(){
  metalSensorValue = analogRead(metalSensorPin);
  // print out the value you read:
  Serial.println(metalSensorValue);
  if(metalSensorValue > 2000) isMetal = true;
}

void readUltrasonicSeedStudio(){  
    int f1L,f2L,f3L,f4L;
    f1L = ultrasonic1.MeasureInCentimeters();
    f2L = ultrasonic2.MeasureInCentimeters();
    f3L = ultrasonic3.MeasureInCentimeters();
    f4L = ultrasonic4.MeasureInCentimeters();
    if(f1L!=0) f1 = f1L;
    if(f2L!=0) f2 = f2L;
    if(f3L!=0) f3 = f3L;
    if(f4L!=0) f4 = f4L;
    // Serial.print("F1:");
    // Serial.println(f1);
    // Serial.print("F2:");
    // Serial.println(f2);
    // Serial.print("F3:");
    // Serial.println(f3);
    // Serial.print("F4:");
    // Serial.println(f4);
}

void readNormalUltrasonic(){
  frontS = readUltrasonicBasic(trigPin1, echoPin1);
  rightS = readUltrasonicBasic(trigPin2, echoPin2);
  leftS =  readUltrasonicBasic(trigPin3, echoPin3);
  // Serial.print("frontS:");
  // Serial.println(frontS);
  // Serial.print("rightS:");
  // Serial.println(rightS);
  // Serial.print("leftS:");
  // Serial.println(leftS);
}

float readUltrasonicBasic(int trigPin, int echoPin){
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration;
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculate the distance
  return duration * SOUND_SPEED/2;
}

void setServoDefaultDown(){
  pwm.setPWM(0, 0, angleToPulse(155));//+left
  pwm.setPWM(1, 0, angleToPulse(80)); //+up
  pwm.setPWM(2, 0, angleToPulse(140));//-up
  pwm.setPWM(3, 0, angleToPulse(30)); //+up
  //Gripper
  pwm.setPWM(4, 0, angleToPulse(80)); //- open
  pwm.setPWM(5, 0, angleToPulse(80)); //+ open
}

void setServoDefaultUp(){
  pwm.setPWM(0, 0, angleToPulse(155));//+left
  pwm.setPWM(1, 0, angleToPulse(180)); //+up
  pwm.setPWM(2, 0, angleToPulse(180));//-up
  pwm.setPWM(3, 0, angleToPulse(0)); //+up
  //Gripper
  pwm.setPWM(4, 0, angleToPulse(80)); //- open
  pwm.setPWM(5, 0, angleToPulse(80)); //+ open
}

int angleToPulse(int ang){
   int pulse = map(ang, 0, 180, SERVOMIN, SERVOMAX);// map angle of 0 to 180 to Servo min and Servo max 
  //  Serial.print("Angle: "); Serial.print(ang);
  //  Serial.print(" pulse: "); Serial.println(pulse);
   return pulse;
}

void moveForward(){
  motor1.setSpeed(255);  // Motor 1 runs forward at 50% speed.
  motor2.setSpeed(255);  // Motor 2 runs backward at 50% speed.
  motor3.setSpeed(255);  // Motor 2 runs backward at 50% speed.
  motor4.setSpeed(255);
}

void moveBackward(){
  motor1.setSpeed(-255);  // Motor 1 runs forward at 50% speed.
  motor2.setSpeed(-255);  // Motor 2 runs backward at 50% speed.
  motor3.setSpeed(-255);  // Motor 2 runs backward at 50% speed.
  motor4.setSpeed(-255);
}

void moveLeft(){
  motor1.setSpeed(255);  // Motor 1 runs forward at 50% speed.
  motor2.setSpeed(-255);  // Motor 2 runs backward at 50% speed.
  motor3.setSpeed(255);  // Motor 2 runs backward at 50% speed.
  motor4.setSpeed(-255);
}

void moveRight(){
  motor1.setSpeed(-255);  // Motor 1 runs forward at 50% speed.
  motor2.setSpeed(255);  // Motor 2 runs backward at 50% speed.
  motor3.setSpeed(-255);  // Motor 2 runs backward at 50% speed.
  motor4.setSpeed(255);
}

void moveLeftSide(){
  motor1.setSpeed(255);  // Motor 1 runs forward at 50% speed.
  motor2.setSpeed(-255);  // Motor 2 runs backward at 50% speed.
  motor3.setSpeed(-255);  // Motor 2 runs backward at 50% speed.
  motor4.setSpeed(255);
}

void moveRightSide(){
  motor1.setSpeed(-255);  // Motor 1 runs forward at 50% speed.
  motor2.setSpeed(255);  // Motor 2 runs backward at 50% speed.
  motor3.setSpeed(255);  // Motor 2 runs backward at 50% speed.
  motor4.setSpeed(-255);
}

void stopMovement(){
  motor1.setSpeed(0);  // Motor 1 runs forward at 50% speed.
  motor2.setSpeed(0);  // Motor 2 runs backward at 50% speed.
  motor3.setSpeed(0);  // Motor 2 runs backward at 50% speed.
  motor4.setSpeed(0);
}
