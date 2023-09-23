#include <Arduino.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <WebSocketsServer.h>

#include "RobotStats.h"
#include "global-var.h"
#include "UtilString.h"
#include "UserCommand.h"

WebSocketsServer server(9090);
boolean hasClient = false;
UserCommand userCommand;

void sendMessage(String msg)
{
    server.sendTXT(userCommand.userId, msg);
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
    float ratioA = (float)rStat->targetLeftVel / rStat->leftVel;
    float ratioB = (float)rStat->targetRightVel / rStat->rightVel;
    sendEncoderMessage2('R', ratioA, ratioB);
    sendEncoderMessage('S', rStat->targetLeftVel, rStat->targetRightVel);
    sendEncoderMessage('E', rStat->leftVel, rStat->rightVel);
    sendEncoderMessage('P', rStat->leftPos, rStat->rightPos);
}

void convertCommand(char *input, UserCommand *uCmd)
{
    char cp1[10], cp2[10];
    uCmd->command = *input;
    tokenize2(input + 1, ',', cp1, cp2);
    uCmd->p1 = atoi2(cp1);
    uCmd->p2 = atoi2(cp2);
    uCmd->ts = millis();
}

void handleCommand(char *input)
{
    Serial.printf("command = <%s>\n", input);
    if (strlen(input) > 1)
        convertCommand(input, &userCommand);
}

UserCommand *getLastCommand()
{
    return &userCommand;
}

void setupWifi()
{
    if (!WiFi.begin(WIFI_SSID, WIFI_PASSWORD))
        Serial.println("connecting to wifi failed.");

    while (!WiFi.isConnected())
    {
        sleep(1);
        Serial.println("Waiting for wifi to be connected...");
    }
    Serial.println("Connected to wifi!");
    Serial.print("IP address:");
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
        userCommand.userId = num;
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\tClient ID = %d\n",
                      num, ip[0], ip[1], ip[2], ip[3], payload, userCommand.userId);
    }
    break;
    case WStype_TEXT:
        handleCommand((char *)payload);
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
    if ((millis() - userCommand.ts) > 10000)
    {
        userCommand.command = 'M';
        userCommand.p1 = 0;
        userCommand.p2 = 0;
        userCommand.ts = millis();
        Serial.println("Connection Lost");
    }
}
