#include <Arduino.h>
#include <WebSocketsServer.h>
#include <WiFi.h>
#include <WiFiAP.h>

int AIN1_PIN = 14;
int AIN2_PIN = 12;
int PWMA_PIN = 27;

int BIN1_PIN = 33;
int BIN2_PIN = 25;
int PWMB_PIN = 32;

int STBY_PIN = 26;

unsigned long COMMAND_EXPIRY_MS = 5000;
unsigned long COMMAND_READ_TIMEOUT = 1;

void setupSerial()
{
    Serial.begin(115200);
    Serial.setTimeout(COMMAND_READ_TIMEOUT);
}

void cwOrCCW(int cwA, int cwB)
{
    // TODO: print message about whether we're turning clockwise
    if (cwA)
    {
        digitalWrite(AIN1_PIN, LOW);
        digitalWrite(AIN2_PIN, HIGH);
    }
    else
    {
        digitalWrite(AIN1_PIN, HIGH);
        digitalWrite(AIN2_PIN, LOW);
    }

    if (cwB)
    {
        digitalWrite(BIN1_PIN, LOW);
        digitalWrite(BIN2_PIN, HIGH);
    }
    else
    {
        digitalWrite(BIN1_PIN, HIGH);
        digitalWrite(BIN2_PIN, LOW);
    }
}

void setupPins()
{
    // output pin must be configured as outputs
    pinMode(AIN1_PIN, OUTPUT);
    pinMode(AIN2_PIN, OUTPUT);
    pinMode(BIN1_PIN, OUTPUT);
    pinMode(BIN2_PIN, OUTPUT);
    pinMode(STBY_PIN, OUTPUT);

    // setup PWM pin for pwm outputs
    pinMode(PWMA_PIN, OUTPUT);
    pinMode(PWMB_PIN, OUTPUT);
}

void standby(int shouldStandby)
{
    // shouldStandBy = 1: standby
    // shouldStandBy = 0: let the motor move
    if (shouldStandby)
    {
        // standby need to be low i.e. 0 to activate
        digitalWrite(STBY_PIN, 0);
    }
    else
    {
        digitalWrite(STBY_PIN, 1);
    }
}

int TURN_LEFT = 1;
int TURN_RIGHT = 2;
int TURN_NONE = 0;
int TURN_AMOUNT_MAX = 100;

/// @brief Drive the robot by supplying different parameters
/// @param velocity Drive forward
/// @param turnDirection 0 for left; 1 for right. 2 for not turning
/// @param turnAmount 0 - 100; Larger the number, the more you turn
void drive(int velocity, int turnDirection = TURN_NONE, int turnAmount = 0)
{
    if (turnDirection == TURN_LEFT)
    {
        standby(0);
        cwOrCCW(1, 0);

        analogWrite(PWMA_PIN, abs(velocity) * 1.02);
        analogWrite(PWMB_PIN, abs(velocity));
    }
    else if (turnDirection == TURN_RIGHT)
    {
        standby(0);
        cwOrCCW(0, 1);

        analogWrite(PWMA_PIN, velocity * 1.02);
        analogWrite(PWMB_PIN, velocity);
    }
    else if (turnDirection == TURN_NONE)
    {
        if (velocity < 0)
        {
            standby(0);
            cwOrCCW(1, 1);

            analogWrite(PWMA_PIN, abs(velocity) * 1.02);
            analogWrite(PWMB_PIN, abs(velocity));
        }
        else if (velocity > 0)
        {
            standby(0);
            cwOrCCW(0, 0);

            analogWrite(PWMA_PIN, abs(velocity * 1.02));
            analogWrite(PWMB_PIN, abs(velocity));
        }
        else
        {
            standby(1);
        }
    }
}

/// @brief
/// @param input
///         it takes 2 commands
///         1. V: V100 drive the robot forward, V-50 drive it backward (but slower), etc
///         2. T: T0 keep the robot straight, T100 to turn robot all the way to the right, T-100 all the way to the left

int handleVelocity = 0;
int handleTurnDirection = TURN_NONE;
int handleTurnAmount = 0;

int timeOfLastCommand = 0;

void handleCommand(String input)
{

    // Serial.println("command from readCommand:");
    // Serial.println(input);

    if (input != "V0" && input != "T0")
    {
        // Serial.println("Command sent");
        timeOfLastCommand = millis();
    }

    if (input[0] == 'V')
    {
        handleVelocity = input.substring(1).toInt();
    }
    else if (input[0] == 'T')
    {
        if (input.substring(1).toInt() == 2)
        {
            handleVelocity = 10;
            handleTurnDirection = 2;
        }
        if (input.substring(1).toInt() == 1)
        {
            handleVelocity = 10;
            handleTurnDirection = TURN_LEFT;
        }
    }

    // TODO: write code so that command expires after certain time
}

WebSocketsServer server(9090);

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

void setup()
{
    setupSerial();
    setupPins();
    standby(1);
    setupWifi();
    setupWebsocketServer();
}

void loop()
{
    if ((millis() - timeOfLastCommand) > 50)
    {
        handleVelocity = 0;
        handleTurnDirection = TURN_NONE;
    }
    drive(handleVelocity, handleTurnDirection, handleTurnAmount);
    server.loop();
}