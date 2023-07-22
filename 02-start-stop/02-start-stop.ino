#include <Arduino.h>


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
  // output pin must be configured as outputs

  pinMode(STANDBY_PIN, OUTPUT);

  pinMode(PWMA_PIN, OUTPUT);
  pinMode(AIN1_PIN, OUTPUT);
  pinMode(AIN2_PIN, OUTPUT);

  pinMode(PWMB_PIN, OUTPUT);
  pinMode(BIN1_PIN, OUTPUT);
  pinMode(BIN2_PIN, OUTPUT);

}

void turnMotorPower(int onOff_IN1, int onOff_IN2, int onOff_PWM, int onOff_STANDBY)
{
  digitalWrite(STANDBY_PIN, onOff_STANDBY);

  digitalWrite(PWMA_PIN, onOff_PWM);
  digitalWrite(AIN1_PIN, onOff_IN1);
  digitalWrite(AIN2_PIN, onOff_IN2);

  digitalWrite(PWMB_PIN, onOff_PWM);
  digitalWrite(BIN1_PIN, onOff_IN1);
  digitalWrite(BIN2_PIN, onOff_IN2);
}

void changeToCw(int cw)
{
  // TODO: print message about whether we're turning clockwise
  if (cw == 1)
  {
    Serial.println("Turning clockwise");
    // TODO: turn clockwise
    turnMotorPower(HIGH, LOW, HIGH, HIGH);
  }
  else if (cw == 0)
  {
    Serial.println("Turning counter clockwise");
    // TODO: turn counter-clockwise
    turnMotorPower(LOW, HIGH, HIGH, HIGH);
  }
}

void standby(int shouldStandby, int time)
{
  // shouldStandBy = 1: standby
  // shouldStandBy = 0: let the motor move

  // TODO: complete the standby function

  if (shouldStandby) 
  {
    turnMotorPower(LOW, LOW, LOW, LOW);
    delay(time);
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
  // TODO: call functions to complete the sequence

  changeToCw(1);
  delay(1000);
  standby(1, 1000);
  changeToCw(0);
  delay(1000);
  standby(1, 1000);
}