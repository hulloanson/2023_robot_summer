#include <Arduino.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <WebSocketsServer.h>
#include <iostream>

#include "global-var.h"

WebSocketsServer server(9090);

boolean hasClient = false;
uint8_t currentClientId = 0;

void setupWifi()
{
    if (!WiFi.begin("doghouse", "22512251"))
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

void updateDisplay(String speed)
{
    // encoder speed (E:), encoder pos (P:), encoder target (A:), enocoder ratio (R:)

    String msgSpeed = "E:" + String(encoderSpeedA) + "," + String(encoderSpeedB);
    String msgPosition = "P:" + String(currentEncoderA) + "," + String(currentEncoderB);

    double ratioA = speed.toInt() / encoderSpeedA;
    double ratioB = speed.toInt() / encoderSpeedB;

    Serial.printf("Ratio a %d. Ratio b %d. \n", ratioA, ratioB);

    String msgRatio = "R:" + String(ratioA) + "," + String(ratioB);

    server.sendTXT(currentClientId, msgSpeed);
    server.sendTXT(currentClientId, msgPosition);
    server.sendTXT(currentClientId, speed);
    server.sendTXT(currentClientId, msgRatio);
}

void handleCommand(String input)
{
    // Serial.print("handleCommand: got input:");
    // Serial.println(input);
    if (input != "V0" || input != "T0")
    {
        // Serial.println("Command sent");
        timeOfLastCommand = millis();
    }

    if (input.length() < 2)
    {
        Serial.println("Command too short.");
        return;
    }
    char command = input[0];
    String data = input.substring(1);
    if (command == 'V')
    {
        handleVelocity = data.toInt();
    }
    else if (command == 'T')
    {
        int turnValue = data.toInt();
        if (turnValue < 0)
        {
            handleTurnDirection = TURN_LEFT;
        }
        else if (turnValue > 0)
        {
            handleTurnDirection = TURN_RIGHT;
        }
        else
        {
            handleTurnDirection = TURN_NONE;
        }
        handleTurnAmount = abs(turnValue);
    }
    updateDisplay(data);
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
        currentClientId = num;
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

void sendMessage(String msg)
{
    server.sendTXT(currentClientId, msg);
}

void setupWebsocketServer()
{
    server.begin();
    server.onEvent(webSocketEvent);
}

void runServerLoop() { server.loop(); }