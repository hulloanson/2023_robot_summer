#include <Arduino.h>

int PWMA_PIN = 32;
int PWMB_PIN = 26;

void setupSerial()
{
  Serial.begin(115200);
}

void setupPins()
{
  // output pin must be configured as outputs
  pinMode(PWMA_PIN, OUTPUT);
  pinMode(PWMB_PIN, OUTPUT);
}

void turnMotorPower(int onOff)
{
  Serial.println("Turning motor power to:");
  Serial.println(onOff);
  digitalWrite(PWMA_PIN, onOff);
  digitalWrite(PWMB_PIN, onOff);
}

void changeToCw(int cw)
{
  // TODO: print message about whether we're turning clockwise
  if (cw == 1)
  {
    // TODO: turn clockwise
  }
  else if (cw == 0)
  {
    // TODO: turn counter-clockwise
  }
}

void standby(int shouldStandby)
{
  // shouldStandBy = 1: standby
  // shouldStandBy = 0: let the motor move

  // TODO: complete the standby function
}

void setup()
{
  setupSerial();
  setupPins();
}

void loop()
{
  /* Sequence:
  1. Turn clockwise for 1 second
  2. Turn off for 1 second
  3. Turn counter-clockwise for 1 second
  4. Standby for 1 second
  */
  // TODO: call functions to complete the sequence
}