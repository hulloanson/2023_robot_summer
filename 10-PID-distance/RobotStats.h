#ifndef RobotStats_h
#define RobotStats_h

#include <Arduino.h>

class RobotStats
{
public:
  int targetLeftVel;
  int targetRightVel;
  int leftVel;
  int rightVel;

  int leftPos;
  int rightPos;

  unsigned long lastCalcTime;

  RobotStats();
  void SerialPrint(String prefix);
};

#endif