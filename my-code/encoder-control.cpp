#include <ESP32Encoder.h>
#include <Arduino.h>

#include "global-var.h"

int encoderSpeedA;
int encoderSpeedB;

ESP32Encoder encoderA;
ESP32Encoder encoderB;

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
}

double *calcEncoder()
{
    double deA = abs(currentEncoderA - prevCountA);
    double deB = abs(currentEncoderB - prevCountB);
    currentTime = millis();
    double dt = (currentTime - lastIntervalTime);

    double speedA = deA / dt;
    double speedB = deB / dt;

    double calculations[3] = {speedA, speedB, dt};
    return calculations;
}

void readEncoder()
{
    currentEncoderA = encoderA.getCount();
    currentEncoderB = encoderB.getCount();

    // speedA = calcEncoder

    if (calcEncoder()[2] == 100)
    {
        Serial.printf("Speed/velocity A is %f. dt is %f. \n", calcEncoder()[0], calcEncoder()[2]);
        Serial.printf("Speed/velocity B is %f. dt is %f. \n", calcEncoder()[1], calcEncoder()[2]);
        lastIntervalTime = currentTime;
        prevCountA = currentEncoderA;
        prevCountB = currentEncoderB;
    }
}
