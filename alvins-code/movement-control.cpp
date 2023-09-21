#include <Arduino.h>
#include <WebSocketsServer.h>

#include "global-var.h"
#include "pid-control.h"
#include "encoder-control.h"

int AIN1_PIN = 14;
int AIN2_PIN = 12;
int PWMA_PIN = 27;
int PWMA_CHANNEL = 0;

int BIN1_PIN = 33;
int BIN2_PIN = 25;
int PWMB_PIN = 32;
int PWMB_CHANNEL = 1;

int STBY_PIN = 26;

int PWM_FREQ = 5000;
int PWM_RESOLUTION = 10;

int MAX_DUTY_CYCLE = (int)(pow(2, PWM_RESOLUTION) - 1);

void setupPwmPin(uint8_t pin, uint8_t channel)
{
    ledcSetup(PWMA_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(pin, channel);
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

double aSpeedFactor = 1;
double bSpeedFactor = 1;

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

void spinMotorAdjusted(int channel, int speed)
{
    ledcWrite(channel, speed);
    return;
    double factor = 1;
    if (channel == PWMA_CHANNEL)
    {
        factor = aSpeedFactor;
    }
    else if (channel == PWMB_CHANNEL)
    {
        factor = bSpeedFactor;
    }
    ledcWrite(channel, int(double(speed) * factor));
}

int targetSpeedA = 0, targetSpeedB = 0;
double aRatio = 0, bRatio = 0;

/// @brief Drive the robot by supplying different parameters
/// @param velocity Drive forward
/// @param turnDirection 0 for left; 1 for right. 2 for not turning
/// @param turnAmount 0 - 100; Larger the number, the more you turn
void drive(int velocity, int turnDirection = TURN_NONE, int turnAmount = 0)
{
    if (velocity == 0)
    {
        standby(1);
    }
    else
    {
        standby(0);
    }
    // turnAmount = 0 - 100
    int slowChannel, fastChannel;
    int clampedTurnAmount = min(TURN_AMOUNT_MAX, abs(turnAmount));
    int speed = abs(velocity);
    int slowChannelSpeed = speed * (TURN_AMOUNT_MAX - clampedTurnAmount) / TURN_AMOUNT_MAX;
    int fastChannelSpeed = speed;

    if (turnDirection == TURN_LEFT)
    {
        targetSpeedA = slowChannelSpeed;
        targetSpeedB = fastChannelSpeed;
    }
    else if (turnDirection == TURN_RIGHT)
    {
        targetSpeedA = fastChannelSpeed;
        targetSpeedB = slowChannelSpeed;
    }
    else
    {
        targetSpeedA = speed;
        targetSpeedB = speed;
    }

    if (velocity < 0)
    {
        // Serial.println("Setting motor directions to backward");
        setDirection(DIRECTION_BACKWARD);
    }
    else
    {
        // Serial.println("Setting motor directions to forward");
        setDirection(DIRECTION_FORWARD);
    }
    // ledcWrite(slowChannel, slowChannelSpeed);
    // ledcWrite(fastChannel, fastChannelSpeed);
    spinMotorAdjusted(PWMA_CHANNEL, targetSpeedA);
    spinMotorAdjusted(PWMB_CHANNEL, targetSpeedB);
}

void velocityFunc(double velo, int channel)
{
    double lastError = 0;
    double lastInput = 0;
    double currentVelo;

    while (true)
    {
        /*
        if (channel)
            currentVelo = calcEncoder()[1];
        else
            currentVelo = calcEncoder()[0];
        */
        if (abs(currentVelo) <= (velo - 0.5))
        {
            break;
        }
        double output = calcPIDOutput2(velo, currentVelo, lastError, lastInput);
        ledcWrite(channel, output);
    }

    ledcWrite(channel, 0);
}

void setVelo(double velo)
{
    velocityFunc(velo, PWMA_CHANNEL);
    velocityFunc(velo, PWMB_CHANNEL);
}

/*
void move(int desiredDistance)
{
    bool moving = true;

    double accL = 0;
    double accR = 0;
    double prevL = 0;
    double prevR = 0;

    while (moving)
    {
        double outputL = calcPIDOutput(desiredDistance, accL, accR, prevL, prevR)[0];
        double outputR = calcPIDOutput(desiredDistance, accL, accR, prevL, prevR)[1];

        if (outputL > 255)
            outputL = 255;
        else if (outputL < -255)
            outputL = -255;
        if (outputR > 255)
            outputL = 255;
        if (outputR < -255)
            outputL = -255;

        drive(outputL);
    }
}
*/