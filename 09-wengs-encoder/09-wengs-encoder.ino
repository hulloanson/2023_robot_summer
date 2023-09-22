#include <Arduino.h>

#include "movement-control.h"
#include "wifi-setup.h"
#include "encoder-control.h"
#include "RobotStats.h"
#include "test.h"

void setupSerial()
{
    Serial.begin(115200);
    Serial.setTimeout(1);
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
    unsigned long timeOfLastCommand = getLastCommandTime();
    int targetVelocity = getTargetVelocity();

    move(targetVelocity);
    // drive(handleVelocity, handleTurnDirection, handleTurnAmount);
    RobotStats *stats = calcEncoder(targetVelocity);
    // stats->SerialPrint("main");
    updateDisplay(stats);

    runServerLoop();
    delay(10);
}