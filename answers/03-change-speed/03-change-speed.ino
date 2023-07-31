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

void setup()
{
  setupSerial();

  setupPins();

  // deactivate standby
  standby(0);

  // change to clockwise
  changeToCw(1);
}

int FAST = MAX_DUTY_CYCLE;
int SLOW = MAX_DUTY_CYCLE / 2;
int STOP = 0;

void loop()
{
  /* TODO: complete this sequence:
  1. Spin slow for 3 seconds
  2. Stop for 3 seconds
  3. Spin fast for 3 seconds
  4. Stop for 3 seconds

  Also print messages in serial to indicate what's going on
  */
  Serial.println("slow..");
  ledcWrite(PWMA_CHANNEL, SLOW);
  ledcWrite(PWMB_CHANNEL, SLOW);
  delay(3000);
  Serial.println("stop.");
  ledcWrite(PWMA_CHANNEL, 0);
  ledcWrite(PWMB_CHANNEL, 0);
  delay(3000);
  Serial.println("fast!");
  ledcWrite(PWMA_CHANNEL, FAST);
  ledcWrite(PWMB_CHANNEL, FAST);
  delay(3000);
  Serial.println("stop.");
  ledcWrite(PWMA_CHANNEL, 0);
  ledcWrite(PWMB_CHANNEL, 0);
  delay(3000);
}
