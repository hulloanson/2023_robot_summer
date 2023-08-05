#include <Arduino.h>
#include <WebSocketsServer.h>
#include <WiFi.h>
#include <WiFiAP.h>

int PWM_FREQ = 5000;
int PWM_RESOLUTION = 10;

int PWMA_CHANNEL = 0;

int PWMA_PIN = 27;

int PWMB_CHANNEL = 1;

int PWMB_PIN = 32;

int AIN1_PIN = 14;
int AIN2_PIN = 12;

int BIN1_PIN = 33;
int BIN2_PIN = 25;

int STBY_PIN = 26;

int MAX_DUTY_CYCLE = (int)(pow(2, PWM_RESOLUTION) - 1);

unsigned long COMMAND_EXPIRY_MS = 5000;
unsigned long COMMAND_READ_TIMEOUT = 1;

void setupPwmPin(uint8_t pin, uint8_t channel)
{
  ledcSetup(PWMA_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(pin, channel);
}

void setupSerial()
{
  Serial.begin(115200);
  Serial.setTimeout(COMMAND_READ_TIMEOUT);
}

int DIRECTION_FORWARD = 1;
int DIRECTION_BACKWARD = 0;

void setDirection(int forward)
{
  if (forward == DIRECTION_FORWARD)
  {
    // IN1 pins should be high and IN2 should be low to spin clockwise
    digitalWrite(AIN1_PIN, 1);
    digitalWrite(AIN2_PIN, 0);
    digitalWrite(BIN1_PIN, 1);
    digitalWrite(BIN2_PIN, 0);
  }
  else if (forward == DIRECTION_BACKWARD)
  {
    // IN1 pins should be low and IN2 pins should be high to spin counter-clockwise
    digitalWrite(AIN1_PIN, 0);
    digitalWrite(AIN2_PIN, 1);
    digitalWrite(BIN1_PIN, 0);
    digitalWrite(BIN2_PIN, 1);
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
  setupPwmPin(PWMA_PIN, PWMA_CHANNEL);
  setupPwmPin(PWMB_PIN, PWMB_CHANNEL);
}

void standby(int shouldStandby)
{
  // shouldStandBy = 1: standby
  // shouldStandBy = 0: let the motor move
  if (shouldStandby == 1)
  {
    // standby need to be low i.e. 0 to activate
    digitalWrite(STBY_PIN, 0);
  }
  else
  {
    digitalWrite(STBY_PIN, 1);
  }
}

int FAST = MAX_DUTY_CYCLE;
int SLOW = MAX_DUTY_CYCLE / 10;
int STOP = 0;

int TURN_LEFT = 0;
int TURN_RIGHT = 1;
int TURN_NONE = 2;
int TURN_AMOUNT_MAX = 100;

/// @brief Drive the robot by supplying different parameters
/// @param velocity Drive forward
/// @param turnDirection 0 for left; 1 for right. 2 for not turning
/// @param turnAmount 0 - 100; Larger the number, the more you turn
void drive(int velocity, int turnDirection = TURN_NONE, int turnAmount = 0)
{
  // turnAmount = 0 - 100
  int slowChannel, fastChannel;
  int clampedTurnAmount = min(TURN_AMOUNT_MAX, abs(turnAmount));
  int speed = abs(velocity);
  int slowChannelSpeed = speed * (TURN_AMOUNT_MAX - clampedTurnAmount) / TURN_AMOUNT_MAX;
  int fastChannelSpeed = speed;

  if (turnDirection == TURN_LEFT)
  {
    slowChannel = PWMA_CHANNEL;
    fastChannel = PWMB_CHANNEL;
  }
  else if (turnDirection == TURN_RIGHT)
  {
    slowChannel = PWMB_CHANNEL;
    fastChannel = PWMA_CHANNEL;
  }
  else if (turnDirection == TURN_NONE)
  {
    // slow and fast doesn't matter because they are turning in the same speed anyways
    slowChannel = PWMB_CHANNEL;
    fastChannel = PWMA_CHANNEL;
    slowChannelSpeed = fastChannelSpeed;
  }
  else
  {
    Serial.print("Wring turnDirection. Not doing anything");
    return;
  }
  if (velocity < 0)
  {
    // Serial.println("Setting motor directions to backward");
    setDirection(DIRECTION_BACKWARD);
  }
  else
  {
    // Serial.println("Setting motor directions to forward");
    setDirection(DIRECTION_FORWARD);
  }
  // Serial.printf("slowChannelSpeed: %d\n", slowChannelSpeed);
  // Serial.printf("fastChannelSpeed: %d\n", fastChannelSpeed);
  ledcWrite(slowChannel, slowChannelSpeed);
  ledcWrite(fastChannel, fastChannelSpeed);
}

/// @brief
/// @param input
///         it takes 2 commands
///         1. V: V100 drive the robot forward, V-50 drive it backward (but slower), etc
///         2. T: T0 keep the robot straight, T100 to turn robot all the way to the right, T-100 all the way to the left
void handleCommand(String input)
{
  Serial.print("handleCommand: got input:");
  Serial.println(input);
  // TODO: drive robot based on command
  // TODO: write code so that command expires after certain time
}

WebSocketsServer server(9090);

void setupWifi()
{
  // if (!WiFi.softAP("CuteRobot", "00000000"))
  // {
  //   Serial.println("software start up ap failed");
  // }
  if (!WiFi.begin("your wifi", "wifi password"))
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
    Serial.printf("[%u] get Text: %s\n", num, payload);
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

  standby(0);
  setupWifi();
  setupWebsocketServer();
}

void loop()
{
  server.loop();
}