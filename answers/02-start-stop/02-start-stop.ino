#include <Arduino.h>

int PWMA_PIN = 27;
int PWMB_PIN = 32;

int AIN1_PIN = 14;
int AIN2_PIN = 12;

int BIN1_PIN = 33;
int BIN2_PIN = 25;

int STBY_PIN = 26;

void setupSerial()
{
  Serial.begin(115200);
}

void setupPins()
{
  // output pin must be configured as outputs
  pinMode(PWMA_PIN, OUTPUT);
  pinMode(PWMB_PIN, OUTPUT);
  pinMode(AIN1_PIN, OUTPUT);
  pinMode(AIN2_PIN, OUTPUT);
  pinMode(BIN1_PIN, OUTPUT);
  pinMode(BIN2_PIN, OUTPUT);
  pinMode(STBY_PIN, OUTPUT);
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
  if (cw == 1)
  {
    Serial.println("Spining clockwise");
    // IN1 pins should be high and IN2 should be low to spin clockwise
    digitalWrite(AIN1_PIN, 1);
    digitalWrite(AIN2_PIN, 0);
    digitalWrite(BIN1_PIN, 1);
    digitalWrite(BIN2_PIN, 0);
  }
  else if (cw == 0)
  {
    Serial.println("Spining counterclockwise");
    // IN1 pins should be low and IN2 pins should be high to spin counter-clockwise
    digitalWrite(AIN1_PIN, 0);
    digitalWrite(AIN2_PIN, 1);
    digitalWrite(BIN1_PIN, 0);
    digitalWrite(BIN2_PIN, 1);
  }
}

void standby(int shouldStandby)
{
  // shouldStandBy = 1: standby
  // shouldStandBy = 0: let the motor move
  if (shouldStandby == 1)
  {
    // standby need to be low i.e. 0 to activate
    digitalWrite(STBY_PIN, 0);
  }
  else
  {
    digitalWrite(STBY_PIN, 1);
  }
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
  changeToCw(1);
  turnMotorPower(1);
  delay(2000);
  turnMotorPower(0);
  delay(2000);
  changeToCw(0);
  turnMotorPower(1);
  delay(2000);
  standby(1);
  delay(2000);
  standby(0);
}