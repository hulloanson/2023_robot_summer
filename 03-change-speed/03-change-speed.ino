#include <Arduino.h>
#define PWM_FREQ 5000

int PWM_RESOLUTION = 10;

const int PWMA_CHANNEL = 0;
int PWMB_CHANNEL = 1;

int STANDBY_PIN = 26;

#define PWMA_PIN 27
#define AIN1_PIN 14
// int PWMA_PIN = 27;
// int AIN1_PIN = 14;
int AIN2_PIN = 12;

int PWMB_PIN = 32;
int BIN1_PIN = 33;
int BIN2_PIN = 25;

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

/*
void changeToCw(int cw)
{
}
*/

/*

IN1 IN2 PWM STBY Mode |
0 0 1 1 Stop  0
0 1 1 1 CCW   1   
1 0 1 1 CW    2
1 1 1 0 STBY  3
*/

void turnMotorPower(int onOff_IN1, int onOff_IN2, int speed, int onOff_STANDBY)
{
  // Serial.println(Speed);
  

  digitalWrite(STANDBY_PIN, onOff_STANDBY);

  analogWrite(PWMA_PIN, speed*1.02);
  digitalWrite(AIN1_PIN, onOff_IN1);
  digitalWrite(AIN2_PIN, onOff_IN2);

  analogWrite(PWMB_PIN, speed);
  digitalWrite(BIN1_PIN, onOff_IN1);
  digitalWrite(BIN2_PIN, onOff_IN2);
}

void turnMotorPowerMasking(int mode) {
  const int IN1 = (mode & 0b10) > 0 ? HIGH : LOW;
  const int IN2 = (mode & 0b10) > 0 ? HIGH : LOW;
  //const int PWM = (mode & 0b10) > 0 ? HIGH : LOW;
  //const int STBY = (mode & 0b10) > 0 ? HIGH : LOW;
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

void standby(int shouldStandby, int time)
{
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

  Serial.printf("\nMotor CW 3s\n");
  turnMotorPower(LOW, HIGH, 25, HIGH);
  delay(2000);
  Serial.printf("Standby 3s\n");
  standby(1, 3000);
  
  Serial.printf("Motor CCW 3s\n");
  turnMotorPower(HIGH, LOW, 25, HIGH);
  delay(2000);
  Serial.printf("Standby 1s\n");
  standby(1, 1000);
}