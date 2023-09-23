#pragma once
// #ifndef global_var_h
// #define global_var_h

#define WIFI_SSID "doghouse"
#define WIFI_PASSWORD "22512251"
#define COMMAND_EXPIRY_MS 1000

#define TURN_LEFT 1
#define TURN_RIGHT 2
#define TURN_NONE 0
#define TURN_AMOUNT_MAX 100

// extern int handleVelocity;
// extern int handleTurnDirection;
// extern int handleTurnAmount;
// extern int timeOfLastCommand;

// extern int currentEncoderA;
// extern int currentEncoderB;
// extern int startOfCommandTime;
// extern int lastIntervalTime;

// extern int prevCountA;
// extern int prevCountB;

// extern int currentTime;

// move to encoder.h
// extern double encoderSpeedA;
// extern double encoderSpeedB;

// #endif

// struct EncoderStats
// {
//   double velocity;
//   int64_t position;
// };

// struct EncoderStats2
// {
//   double velocity;
//   int64_t position;
// };

// struct RobotStats0
// {
//   int32_t velocitySetPoint;
//   unsigned long lastCalcTime;
//   EncoderStats leftEncoder;
//   EncoderStats2 rightEncoder;
// };

// struct RobotStats
// {
//   int32_t velocitySetPoint;
//   unsigned long lastCalcTime;
//   int64_t leftPos;
//   double leftVel;
//   int64_t rightPos;
//   double rightVel;
// };