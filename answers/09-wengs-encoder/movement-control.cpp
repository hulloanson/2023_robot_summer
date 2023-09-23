#include <Arduino.h>
#include "movement-control.h"
#include "UtilMath.h"

#define PWM_FREQ 5000
#define PWM_RESOLUTION 10
int MAX_DUTY_CYCLE = (int)(pow(2, PWM_RESOLUTION) - 1);

void setupPwmPin(uint8_t pin, uint8_t channel)
{
    // ledcSetup(PWMA_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
    ledcSetup(channel, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(pin, channel);
}

void setDirection(bool leftForward, bool rightForward)
{
    if (leftForward)
    {
        // IN1 pins should be low and IN2 pins should be high to spin counter-clockwise
        digitalWrite(AIN1_PIN, 0);
        digitalWrite(AIN2_PIN, 1);
    }
    else
    {
        // IN1 pins should be high and IN2 should be low to spin clockwise
        digitalWrite(AIN1_PIN, 1);
        digitalWrite(AIN2_PIN, 0);
    }

    if (rightForward)
    {
        digitalWrite(BIN1_PIN, 0);
        digitalWrite(BIN2_PIN, 1);
    }
    else
    {
        digitalWrite(BIN1_PIN, 1);
        digitalWrite(BIN2_PIN, 0);
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

void standby(bool shouldStandby)
{
    // shouldStandBy = 1: standby
    // shouldStandBy = 0: let the motor move
    // standby need to be low i.e. 0 to activate
    digitalWrite(STBY_PIN, shouldStandby ? 0 : 1);
}

int CALBRATION_LEN = 13;
int calibrationX[] = {-1000, -500, -300, -200, -100, -1, 0, 1, 100, 200, 300, 500, 1000};
int calibrationYA[] = {-232, -130, -85, -64, -41, -30, 30, 0, 34, 55, 75, 118, 223};
int calibrationYB[] = {-227, -118, -76, -55, -34, -16, 0, 25, 33, 55, 75, 119, 225};

int lookUpPowerInput(int velocity, int x[], int y[])
{
    if (velocity > x[CALBRATION_LEN - 1])
        return y[CALBRATION_LEN - 1];
    else if (velocity < x[0])
        return y[0];

    for (int i = CALBRATION_LEN - 2; i >= 0; i--)
    {
        if (x[i] < velocity)
            return abs(interpolate(x[i], y[i],
                                   x[i + 1], y[i + 1],
                                   velocity));
    }

    return 0;
}

int getMotorAPowerInput(int velocity)
{
    return lookUpPowerInput(velocity, calibrationX, calibrationYA);
}

int getMotorBPowerInput(int velocity)
{
    return lookUpPowerInput(velocity, calibrationX, calibrationYB);
}

void spinMotorAdjusted2(int channel, int speed)
{
    ledcWrite(channel, speed);
}

int lastLeftVel = 0;
int lastRightVel = 0;

void move(int leftVel, int rightVel)
{
    standby(leftVel == 0 && rightVel == 0);
    if (lastLeftVel != leftVel || lastRightVel != rightVel)
    {
        setDirection(leftVel > 0, rightVel > 0);
        spinMotorAdjusted2(PWMA_CHANNEL, getMotorAPowerInput(leftVel));
        spinMotorAdjusted2(PWMB_CHANNEL, getMotorBPowerInput(rightVel));
        lastLeftVel = leftVel;
        lastRightVel = rightVel;
    }
}

void drive(int velocity)
{
    if (velocity == 0)
    {
        standby(true);
    }
    else
    {
        standby(false);
    }

    setDirection(velocity > 0, velocity > 0);
    int speed = abs(velocity);
    int targetSpeedA = speed;
    int targetSpeedB = speed;

    spinMotorAdjusted2(PWMA_CHANNEL, targetSpeedA);
    spinMotorAdjusted2(PWMB_CHANNEL, targetSpeedB);
}
