#include <Arduino.h>

#include "movement-control.h"
#include "wifi-setup.h"
#include "global-var.h"
#include "encoder-control.h"

int handleVelocity = 0;
int handleTurnDirection = TURN_NONE;
int handleTurnAmount = 0;

int startOfCommandTime = 0;
int timeOfLastCommand = 0;

int currentEncoderA = 0;
int currentEncoderB = 0;

int prevCountA = 0;
int prevCountB = 0;

double encoderSpeedA = 0;
double encoderSpeedB = 0;

int currentTime = 0;
int lastIntervalTime = 0;

unsigned long COMMAND_EXPIRY_MS = 5000;
unsigned long COMMAND_READ_TIMEOUT = 1;

void setupSerial()
{
    Serial.begin(115200);
    Serial.setTimeout(COMMAND_READ_TIMEOUT);
}

void setup()
{
    setupSerial();
    setupPins();
    standby(1);
    setupWifi();
    setupWebsocketServer();
    setupEncoder();
}

void loop()
{

    unsigned long now = millis();
    if ((now - timeOfLastCommand) > 50)
    {
        handleVelocity = 0;
        handleTurnDirection = TURN_NONE;
    }
    drive(handleVelocity, handleTurnDirection, handleTurnAmount);

    calcEncoder();
    // updateDisplay();

    runServerLoop();
}