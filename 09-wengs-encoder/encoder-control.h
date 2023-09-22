#ifndef encoder_control_h
#define encoder_control_h

#include "./RobotStats.h"

void setupEncoder();

RobotStats *calcEncoder(int targetVelocity);

// extern int currentEncoderA;
// extern int currentEncoderB;
// extern int startOfCommandTime;
// extern int lastIntervalTime;

// extern int prevCountA;
// extern int prevCountB;

// extern int currentTime;
// extern double encoderSpeedA;
// extern double encoderSpeedB;

#endif