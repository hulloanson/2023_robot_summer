#ifndef encoder_control_h
#define encoder_control_h

#include "RobotStats.h"
#include "UserCommand.h"

void setupEncoder();

RobotStats *calcEncoder(UserCommand *userCommand);

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