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

void readEncoder()
{
    currentEncoderA = encoderA.getCount();
    currentEncoderB = encoderB.getCount();

    double deA = abs(currentEncoderA - prevCountA);
    double deB = abs(currentEncoderB - prevCountB);
    currentTime = millis();
    double dt = (currentTime - lastIntervalTime);
    if (dt == 100)
    {
        Serial.printf("Speed/velocity A is %f. de is %f. dt is %f. \n", (deA / dt), deA, dt);
        Serial.printf("Speed/velocity B is %f. de is %f. dt is %f. \n", (deB / dt), deB, dt);
        lastIntervalTime = currentTime;
        prevCountA = currentEncoderA;
        prevCountB = currentEncoderB;
    }
}
