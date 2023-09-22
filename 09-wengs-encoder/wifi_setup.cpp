#include <Arduino.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <WebSocketsServer.h>
#include <stdio.h>

#include "RobotStats.h"
#include "global-var.h"

WebSocketsServer server(9090);
boolean hasClient = false;
uint8_t currentClientId = 0;
int handleVelocity = 0;
int handleTurnDirection = 0;
int handleTurnAmount = 0;
unsigned long timeOfLastCommand = 0;

int getTargetVelocity()
{
    return handleVelocity;
}

unsigned long getLastCommandTime()
{
    return timeOfLastCommand;
}

void sendMessage(String msg)
{
    server.sendTXT(currentClientId, msg);
}

void sendEncoderMessage2(char prefix, float leftValue, float rightValue)
{
    char buffer[100];
    snprintf(buffer, 100, "%c:%.3f,%.3f", prefix, leftValue, rightValue);
    sendMessage(buffer);
}

void sendEncoderMessage(char prefix, int leftValue, int rightValue)
{
    char buffer[100];
    snprintf(buffer, 100, "%c:%d,%d", prefix, leftValue, rightValue);
    sendMessage(buffer);
}

void updateDisplay(RobotStats *rStat)
{
    float ratioA = (float)rStat->leftVel / rStat->targetVel;
    float ratioB = (float)rStat->rightVel / rStat->targetVel;
    sendEncoderMessage2('R', ratioA, ratioB);
    sendEncoderMessage('S', rStat->targetVel, rStat->targetVel);
    sendEncoderMessage('E', rStat->leftVel, rStat->rightVel);
    sendEncoderMessage('P', rStat->leftPos, rStat->rightPos);
}

void handleCommand(String input)
{
    if (input != "V0" || input != "T0")
    {
        // Serial.println("Command sent: " + input);
        timeOfLastCommand = millis();
    }

    if (input.length() < 2)
    {
        Serial.println("Command too short.");
        return;
    }

    char command = input[0];
    int value = input.substring(1).toInt();
    if (command == 'V')
    {
        handleVelocity = value;
    }
    // else if (command == 'T')
    // {
    //     handleTurnDirection = value < 0 ? TURN_LEFT : value > 0 ? TURN_RIGHT
    //                                                             : TURN_NONE;
    //     handleTurnAmount = abs(value);
    // }
}

void setupWifi()
{
    if (!WiFi.begin(WIFI_SSID, WIFI_PASSWORD))
    {
        Serial.println("connecting to wifi failed.");
    }
    while (!WiFi.isConnected())
    {
        sleep(1);
        Serial.println("Waiting for wifi to be connected...");
    }
    Serial.println("Connected to wifi!");
    Serial.println("IP address:");
    Serial.println(WiFi.localIP());
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
        Serial.printf("[%u] Disconnected!\n", num);
        break;
    case WStype_CONNECTED:
    {
        IPAddress ip = server.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
    }
    break;
    case WStype_TEXT:
        handleCommand(String(payload, length));
        break;
    case WStype_BIN:
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
        break;
    }
}

void setupWebsocketServer()
{
    server.begin();
    server.onEvent(webSocketEvent);
}

void runServerLoop()
{
    server.loop();
    if ((millis() - timeOfLastCommand) > 1000)
    {
        handleVelocity = 0;
        // handleTurnDirection = TURN_NONE;
        if (currentClientId > 0)
            Serial.println("No Connection");
    }
}
