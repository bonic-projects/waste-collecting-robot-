//Servos
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN  100 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)

int s1Pos = 155;
int s2Pos = 99;
int s3Pos = 107;
int s4Pos = 20;
int s5Pos = 50;//80 closed
int s6Pos = 110;//80 closed

// struct ServoPositions {
//   int finalPos1;
//   int finalPos2;
// };

struct ServoPositions {
  int finalPos1;
  int finalPos2;
  int finalPos3;
};struct ServoPositions {
  int finalPos1;
  int finalPos2;
  int finalPos3;
};

void setup() {
  Serial.begin(115200);
  Serial.println("BOA Starting..");
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  //Default
  // setServoDefaultUp(); 
  pwm.setPWM(0, 0, angleToPulse(s1Pos));//+left
  pwm.setPWM(1, 0, angleToPulse(s2Pos)); //+up
  pwm.setPWM(2, 0, angleToPulse(s3Pos));//-up
  pwm.setPWM(3, 0, angleToPulse(s4Pos)); //+up
  //Gripper
  pwm.setPWM(4, 0, angleToPulse(s5Pos)); //- open
  pwm.setPWM(5, 0, angleToPulse(s6Pos)); //+ open
}

void loop() {
  delay(1000);
  pickWaste(true, false);
  delay(1000);
  dropWaste();

}

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
  Serial.println(s5Pos);
  Serial.println(s6Pos);
  Serial.println(s3Pos);
}

void dropWaste(){
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


// ServoPositions moveTwoServosSlowly(int servoNum1, int startPos1, int servoNum2, int startPos2, int finalAngle1, int finalAngle2, int stepDelay) {
//   int currentPos1 = startPos1;
//   int currentPos2 = startPos2;
  
//   // Determine whether to increment or decrement for each servo
//   int increment1 = (finalAngle1 > startPos1) ? 1 : -1;
//   int increment2 = (finalAngle2 > startPos2) ? 1 : -1;

//   // Loop until both servos reach their final angles
//   while (currentPos1 != finalAngle1 || currentPos2 != finalAngle2) {
//     // Update the current positions for both servos
//     if (currentPos1 != finalAngle1) {
//       currentPos1 += increment1;
//     }
//     if (currentPos2 != finalAngle2) {
//       currentPos2 += increment2;
//     }

//     // Set the positions for both servos
//     pwm.setPWM(servoNum1 - 1, 0, angleToPulse(currentPos1));
//     pwm.setPWM(servoNum2 - 1, 0, angleToPulse(currentPos2));

//     delay(stepDelay); // Delay for smooth movement
//   }
  
//   // Create a ServoPositions structure and assign final positions
//   ServoPositions finalPositions;
//   finalPositions.finalPos1 = currentPos1;
//   finalPositions.finalPos2 = currentPos2;
  
//   return finalPositions;
// }




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
  pwm.setPWM(1, 0, angleToPulse(140)); //+up
  pwm.setPWM(2, 0, angleToPulse(100));//-up
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
