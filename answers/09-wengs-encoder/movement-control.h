#ifndef movement_control_h
#define movement_control_h

// int AIN1_PIN = 14;
// int AIN2_PIN = 12;
// int PWMA_PIN = 27;
// int PWMA_CHANNEL = 0;

// int BIN1_PIN = 33;
// int BIN2_PIN = 25;
// int PWMB_PIN = 32;
// int PWMB_CHANNEL = 1;

// int STBY_PIN = 26;

// int PWM_FREQ = 5000;
// int PWM_RESOLUTION = 10;
// int MAX_DUTY_CYCLE = (int)(pow(2, PWM_RESOLUTION) - 1);

// int DIRECTION_FORWARD = 1;
// int DIRECTION_BACKWARD = 0;

#define DIRECTION_FORWARD 1
#define DIRECTION_BACKWARD 0

#define AIN1_PIN 14
#define AIN2_PIN 12
#define PWMA_PIN 27
#define PWMA_CHANNEL 0

#define BIN1_PIN 33
#define BIN2_PIN 25
#define PWMB_PIN 32
#define PWMB_CHANNEL 1

#define STBY_PIN 26

void standby(bool shouldStandby);

void move(int leftVel, int rightVel);

void drive(int velocity);
// , int turnDirection = 0, int turnAmount = 0);

void setupPins();

#endif