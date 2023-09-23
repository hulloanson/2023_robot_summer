#include <Arduino.h>

#include "movement-control.h"
#include "wifi-setup.h"
#include "encoder-control.h"
#include "RobotStats.h"

void setupSerial()
{
    Serial.begin(115200);
    Serial.setTimeout(1);
}

void setup()
{
    setupSerial();
    setupPins();
    standby(true);
    setupWifi();
    setupWebsocketServer();
    setupEncoder();
}

unsigned long lastPrintTs = 0;
void loop()
{
    UserCommand *userCommand = getLastCommand();

    if (userCommand->command == 'M')
        move(userCommand->p1, userCommand->p2);
    else if (userCommand->command == 'V')
        move(userCommand->p1, userCommand->p1);

    RobotStats *stats = calcEncoder(userCommand);
    updateDisplay(stats);
    runServerLoop();

    if (lastPrintTs < userCommand->ts)
    {
        Serial.printf("Main\tAddress: %p\tCommand: %c\tP1 = %d\tP2 = %d\tts = %ld\n",
                      userCommand, userCommand->command, userCommand->p1, userCommand->p2, userCommand->ts);
        stats->SerialPrint("Main");
        lastPrintTs = userCommand->ts;
    }
    delay(10);
}