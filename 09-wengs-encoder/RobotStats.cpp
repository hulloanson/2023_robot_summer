#include "RobotStats.h"

RobotStats::RobotStats()
{
  targetVel = 0;
  leftVel = 0;
  rightVel = 0;

  leftPos = 0;
  rightPos = 0;

  lastCalcTime = 0;
  SerialPrint("Create Stats");
}

void RobotStats::SerialPrint(String prefix)
{
  Serial.printf("%s\tAddress: %p\tTargetVel: %d\tleftPos: %d\trightPos: %d\tleftVel: %d\trightVel: %d\tlastCalc: %ld\n",
                prefix, this, targetVel, leftPos, rightPos, leftVel, rightVel, lastCalcTime);
}