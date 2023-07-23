#include <Arduino.h>

// include code from 03-change-speed

int TURN_LEFT = 0;
int TURN_RIGHT = 1;
int TURN_NONE = 2;
int TURN_AMOUNT_MAX = 100;

/// @brief Drive the robot by supplying different parameters
/// @param velocity Drive forward
/// @param turnDirection 0 for left; 1 for right. 2 for not turning
/// @param turnAmount 0 - 100; Larger the number, the more you turn
void drive(int velocity, int turnDirection = TURN_NONE, int turnAmount = 0)
{
  // TODO: finish this
}

void setup()
{
  // include code from 03-change-speed
}

void loop()
{
  /* TODO: complete this sequence:
  Run each step for 1 second.
  1. Drive forward
  2. Drive backward
  3. Drive left, forward
  3. Drive left, backward
  4. Drive right, forward
  5. Drive right, backward

  Also print messages in serial to indicate what's going on
  */
}