#ifndef RobotStats_h
#define RobotStats_h

#include <Arduino.h>

class RobotStats
{
public:
  int targetVel;
  int leftVel;
  int rightVel;

  int leftPos;
  int rightPos;

  unsigned long lastCalcTime;

  RobotStats();
  void SerialPrint(String prefix);
};

#endif