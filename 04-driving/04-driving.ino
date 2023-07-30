#include <Arduino.h>

// include code from 03-change-speed

int TURN_LEFT = 0;
int TURN_RIGHT = 1;
int TURN_NONE = 2;
int TURN_AMOUNT_MAX = 100;

int STANDBY_PIN = 26;

int PWMA_PIN = 27;
int AIN1_PIN = 14;
int AIN2_PIN = 12;

int PWMB_PIN = 32;
int BIN1_PIN = 33;
int BIN2_PIN = 25;

void setupSerial()
{
  Serial.begin(115200);
}

void setupPins()
{
  pinMode(STANDBY_PIN, OUTPUT);

  pinMode(PWMA_PIN, OUTPUT);
  pinMode(AIN1_PIN, OUTPUT);
  pinMode(AIN2_PIN, OUTPUT);

  pinMode(PWMB_PIN, OUTPUT);
  pinMode(BIN1_PIN, OUTPUT);
  pinMode(BIN2_PIN, OUTPUT);
}

void cwOrCCW(int cw)
{
  // TODO: print message about whether we're turning clockwise
  if (cw)
  {
    Serial.println("Turning clockwise");
    // TODO: turn clockwise
    digitalWrite(AIN1_PIN, LOW);
    digitalWrite(AIN2_PIN, HIGH);
    digitalWrite(BIN1_PIN, LOW);
    digitalWrite(BIN2_PIN, HIGH);
  }
  else
  {
    Serial.println("Turning counter clockwise");
    // TODO: turn counter-clockwise
    digitalWrite(AIN1_PIN, HIGH);
    digitalWrite(AIN2_PIN, LOW);
    digitalWrite(BIN1_PIN, HIGH);
    digitalWrite(BIN2_PIN, LOW);
  }
}

/// @brief Drive the robot by supplying different parameters
/// @param velocity Drive forward
/// @param turnDirection 0 for left; 1 for right. 2 for not turning
/// @param turnAmount 0 - 100; Larger the number, the more you turn
void drive(int velocity, int turnDirection = TURN_NONE, int turnAmount = 0)
{
  // TODO: finish this
  if (velocity > 0)
  {
    Serial.println("Velocity > 0");
    cwOrCCW(1);

    analogWrite(PWMA_PIN, velocity * 1.02);
    analogWrite(PWMB_PIN, velocity);
  }
  else if (velocity < 0)
  {
    Serial.println("Velocity < 0");
    cwOrCCW(0);

    analogWrite(PWMA_PIN, velocity * 1.02);
    analogWrite(PWMB_PIN, velocity);
  }
}

void changeToStandby(int shouldStandby)
{
  // shouldStandBy = 1: standby
  // shouldStandBy = 0: let the motor move

  // TODO: complete the standby function

  if (shouldStandby)
  {
    digitalWrite(STANDBY_PIN, LOW);
  }
  else
  {
    digitalWrite(STANDBY_PIN, HIGH);
  }
}

void setup()
{
  // include code from 03-change-speed
  setupSerial();

  setupPins();

  drive(0, TURN_NONE, 0);
  changeToStandby(true);
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
  changeToStandby(false);
  drive(100, TURN_NONE, 0);
  delay(3000);

  drive(-100, TURN_NONE, 0);
  delay(3000);
}