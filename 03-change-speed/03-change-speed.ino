#include <Arduino.h>

int PWM_FREQ = 5000;
int PWM_RESOLUTION = 10;

int PWMA_CHANNEL = 0;
int PWMB_CHANNEL = 1;

int PWMA_PIN = 32;
int PWMB_PIN = 26;

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
}

void setupPins()
{
}

void standby(int shouldStandby)
{
}

void setup()
{
  setupSerial();

  setupPins();

  // TODO: set up pwm pins
}

void loop()
{
  /* TODO: complete this sequence:
  1. Spin slow for 3 seconds
  2. Stop for 3 seconds
  3. Spin fast for 3 seconds
  4. Stop for 3 seconds

  Also print messages in serial to indicate what's going on

  Hint: refer to arduino-esp32 doc!
  */
}