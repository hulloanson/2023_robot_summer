#pragma once
// #ifndef global_var_h
// #define global_var_h

#define TURN_LEFT 1
#define TURN_RIGHT 2
#define TURN_NONE 0
#define TURN_AMOUNT_MAX 100

extern int handleVelocity;
extern int handleTurnDirection;
extern int handleTurnAmount;
extern int timeOfLastCommand;

extern int currentEncoderA;
extern int currentEncoderB;
extern int startOfCommandTime;
extern int lastIntervalTime;

extern int prevCountA;
extern int prevCountB;

extern double currentTime;

// #endif
