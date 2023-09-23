#include <Arduino.h>

#include "movement-control.h"
#include "wifi-setup.h"
#include "encoder-control.h"
#include "RobotStats.h"
#include "pid-control.h"

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
    setupPids();
    UserCommand *userCommand = getLastCommand();
    userCommand->command = 'S';
    userCommand->p1 = 10000;
    userCommand->p2 = 10000;
    userCommand->ts = millis();
}

unsigned long lastPrintTs = 0;
double kp;
double ki;
double kd;

void loop()
{
    UserCommand *userCommand = getLastCommand();

    // if (userCommand->command == 'M')
    //     move(userCommand->p1, userCommand->p2);
    // else if (userCommand->command == 'V')
    //     move(userCommand->p1, userCommand->p1);
    // else

    if (userCommand->command == 'S') // set PID
    {
        setPidInputs(0, 0);
        setPidSetPoints(userCommand->p1, userCommand->p2);
        setPidMode(true);
        userCommand->command = 'X';
        Serial.println("-- PID Setup Done --");
    }

    RobotStats *stats = calcEncoder(userCommand);
    updateDisplay(stats);

    if (userCommand->command == 'X') // execute PID
    {
        int leftInput = stats->leftPos;
        int rightInput = stats->rightPos;
        setPidInputs(leftInput, rightInput);
        int leftOut, rightOut;
        computePids(&leftOut, &rightOut);
        // stats->SerialPrint("PID");
        Serial.printf("PID: leftIn = %d\trightIn = %d\tleftOut = %d\trightOut = %d\n", leftInput, rightInput, leftOut, rightOut);
        move(leftOut, rightOut);
    }

    runServerLoop();

    if (lastPrintTs < userCommand->ts)
    {
        Serial.printf("Main\tAddress: %p\tCommand: %c\tP1 = %d\tP2 = %d\tts = %ld\n",
                      userCommand, userCommand->command, userCommand->p1, userCommand->p2, userCommand->ts);
        stats->SerialPrint("Main");
        lastPrintTs = userCommand->ts;
    }
    delay(100);
}