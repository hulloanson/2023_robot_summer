#include <Arduino.h>

int PWM_FREQ = 5000;
int PWM_RESOLUTION = 10;

int PWMA_CHANNEL = 0;

int PWMA_PIN = 27;

int PWMB_CHANNEL = 1;

int PWMB_PIN = 32;

int AIN1_PIN = 14;
int AIN2_PIN = 12;

int BIN1_PIN = 33;
int BIN2_PIN = 25;

int STBY_PIN = 26;

int MAX_DUTY_CYCLE = (int)(pow(2, PWM_RESOLUTION) - 1);

void setupPwmPin(uint8_t pin, uint8_t channel)
{
  ledcSetup(PWMA_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(pin, channel);
}

void setupSerial()
{
  Serial.begin(115200);
}

int DIRECTION_FORWARD = 1;
int DIRECTION_BACKWARD = 0;

void setDirection(int forward)
{
  if (forward == DIRECTION_FORWARD)
  {
    // IN1 pins should be high and IN2 should be low to spin clockwise
    digitalWrite(AIN1_PIN, 1);
    digitalWrite(AIN2_PIN, 0);
    digitalWrite(BIN1_PIN, 1);
    digitalWrite(BIN2_PIN, 0);
  }
  else if (forward == DIRECTION_BACKWARD)
  {
    // IN1 pins should be low and IN2 pins should be high to spin counter-clockwise
    digitalWrite(AIN1_PIN, 0);
    digitalWrite(AIN2_PIN, 1);
    digitalWrite(BIN1_PIN, 0);
    digitalWrite(BIN2_PIN, 1);
  }
}

void setupPins()
{
  // output pin must be configured as outputs
  pinMode(AIN1_PIN, OUTPUT);
  pinMode(AIN2_PIN, OUTPUT);
  pinMode(BIN1_PIN, OUTPUT);
  pinMode(BIN2_PIN, OUTPUT);
  pinMode(STBY_PIN, OUTPUT);

  // setup PWM pin for pwm outputs
  setupPwmPin(PWMA_PIN, PWMA_CHANNEL);
  setupPwmPin(PWMB_PIN, PWMB_CHANNEL);
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

int FAST = MAX_DUTY_CYCLE;
int SLOW = MAX_DUTY_CYCLE / 2;
int STOP = 0;

void setSpeed(int left, int right)
{
  ledcWrite(PWMA_CHANNEL, left);
  ledcWrite(PWMB_CHANNEL, right);
}

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
  // turnAmount = 0 - 100
  int slowChannel, fastChannel;
  int clampedTurnAmount = min(TURN_AMOUNT_MAX, abs(turnAmount));
  int speed = abs(velocity);
  int slowChannelSpeed = speed * (TURN_AMOUNT_MAX - clampedTurnAmount) / TURN_AMOUNT_MAX;
  int fastChannelSpeed = speed;

  if (turnDirection == TURN_LEFT)
  {
    slowChannel = PWMA_CHANNEL;
    fastChannel = PWMB_CHANNEL;
  }
  else if (turnDirection == TURN_RIGHT)
  {
    slowChannel = PWMB_CHANNEL;
    fastChannel = PWMA_CHANNEL;
  }
  else if (turnDirection == TURN_NONE)
  {
    // slow and fast doesn't matter because they are turning in the same speed anyways
    slowChannel = PWMB_CHANNEL;
    fastChannel = PWMA_CHANNEL;
    slowChannelSpeed = velocity;
  }
  else
  {
    Serial.print("Wring turnDirection. Not doing anything");
    return;
  }
  if (velocity < 0)
  {
    setDirection(DIRECTION_BACKWARD);
  }
  else
  {
    setDirection(DIRECTION_FORWARD);
  }
  ledcWrite(slowChannel, slowChannelSpeed);
  ledcWrite(fastChannel, fastChannelSpeed);
}

void setup()
{
  setupSerial();

  setupPins();

  // deactivate standby
  standby(0);
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
  Serial.println("Driving forward");
  drive(SLOW);
  delay(1000);
  Serial.println("stop.");
  drive(0, TURN_NONE);
  delay(1000);
  Serial.println("Driving backward");
  drive(0 - SLOW, TURN_NONE);
  delay(1000);
  Serial.println("Turning left and forward");
  drive(SLOW, TURN_LEFT, 50);
  delay(1000);
  Serial.println("Turn left and backward");
  drive(SLOW, TURN_LEFT, 50);
  delay(1000);
  Serial.println("Turn right and forward");
  drive(SLOW, TURN_RIGHT, 50);
  delay(1000);
  Serial.println("Turn right and backward");
  drive(0 - SLOW, TURN_RIGHT, 50);
  delay(1000);
}