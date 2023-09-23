#include <ESP32Encoder.h>
#include <Arduino.h>

#include "encoder-control.h"
#include "RobotStats.h"
#include "UserCommand.h"

ESP32Encoder encoderA;
ESP32Encoder encoderB;
RobotStats robotStats;

int E1A_PIN = 36;
int E1B_PIN = 39;
int E2A_PIN = 34;
int E2B_PIN = 35;

void setupEncoder()
{
    ESP32Encoder::useInternalWeakPullResistors = UP;
    encoderA.attachHalfQuad(E1A_PIN, E1B_PIN);
    encoderA.setCount(0);
    encoderB.attachHalfQuad(E2A_PIN, E2B_PIN);
    encoderB.setCount(0);
    robotStats.lastCalcTime = millis();

    robotStats.SerialPrint("encoderSetup");
}

RobotStats *calcEncoder(UserCommand *userCommand)
{
    unsigned long currentTime = millis();
    double dt = currentTime - robotStats.lastCalcTime;

    if (dt >= 500)
    {
        robotStats.targetLeftVel = userCommand->p1;
        robotStats.targetRightVel = userCommand->p2;
        int prevCountA = robotStats.leftPos;
        int prevCountB = robotStats.rightPos;

        robotStats.leftPos = encoderA.getCount();
        robotStats.rightPos = encoderB.getCount();

        int deA = (robotStats.leftPos - prevCountA) * 1000;
        int deB = (robotStats.rightPos - prevCountB) * 1000;

        robotStats.leftVel = deA / dt;
        robotStats.rightVel = deB / dt;
        robotStats.lastCalcTime = currentTime;
    }

    return &robotStats;
}
