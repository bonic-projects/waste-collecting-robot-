#include "CytronMD.h"

// Constructor
CytronMD::CytronMD(int pwmPin, int dirPin) : pwmPin(pwmPin), dirPin(dirPin)
{
    pinMode(pwmPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
}

// Set speed and direction
void CytronMD::setSpeed(int speed)
{
    // Map speed to the range of 0 to 255
    int mappedSpeed = map(abs(speed), 0, 255, 0, 255);

    // Set direction based on the sign of speed
    if (speed >= 0)
    {
        digitalWrite(dirPin, HIGH); // Forward direction
    }
    else
    {
        digitalWrite(dirPin, LOW); // Reverse direction
    }

    // Write the mapped speed to the PWM pin
    analogWrite(pwmPin, mappedSpeed);
}
