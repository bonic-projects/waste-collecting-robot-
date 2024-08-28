#ifndef CYTRONMD_H
#define CYTRONMD_H

#include <Arduino.h>

class CytronMD
{
private:
    int pwmPin;
    int dirPin;

public:
    // Constructor
    CytronMD(int pwmPin, int dirPin);

    // Set speed and direction
    void setSpeed(int speed);
};

#endif
