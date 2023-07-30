#include <Arduino.h>

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

class CommandTime
{
public:
  CommandTime(unsigned long expiryMs = COMMAND_EXPIRY_MS)
  {
    this->expiryMs = expiryMs;
    this->reset();
  }

private:
  unsigned long expiryMs = 0;
  unsigned long last = INFINITY;

public:
  void refresh()
  {
    last = millis();
  }

  bool isExpired()
  {
    auto time = millis();
    return (time > this->last) && (time - this->last >= this->expiryMs);
  }

  void reset()
  {
    this->last = INFINITY;
  }
};

CommandTime velocityCommandTime;
CommandTime turnCommandTime;

class RobotState
{
public:
  int velocity = 0;
  uint8_t turnDirection = TURN_NONE;
  uint8_t turnAmount = 0;
};

RobotState state;

String buffer;

void readCommand(String &command)
{
  if (Serial.available() == 0)
  {
    return;
  }
  String input = Serial.readString();
  Serial.println("buffer pre-append:");
  Serial.println(buffer);
  buffer += input;
  Serial.println("buffer:");
  Serial.println(buffer);
  int lineEndIndex = buffer.indexOf('\n');
  Serial.printf("lineEndIndex: %d", lineEndIndex);
  if (lineEndIndex >= 0)
  {
    command.remove(0);
    command += buffer.substring(0, lineEndIndex);
    buffer.remove(0, lineEndIndex + 1);
  }
}

void handleCommand(String input)
{
  Serial.printf("handleCommand: got input: %s", input.c_str());
  if (input.length() < 2)
  {
    Serial.println("Command too short.");
    return;
  }
  char command = input[0];
  String data = input.substring(1);
  switch (command)
  {
  case 'V': // velocity
    velocityCommandTime.refresh();
    state.velocity = data.toInt();
    break;  // must break unless you want to match against the next case as well
  case 'T': // turn
    turnCommandTime.refresh();
    state.turnDirection = data.substring(0, 2).toInt();
    state.turnAmount = data.substring(1).toInt();
    break;
  default:
    break;
  }
}

void setup()
{
  setupSerial();

  setupPins();

  standby(0);
}

void resetIfExpired()
{
  if (velocityCommandTime.isExpired())
  {
    Serial.println("velocitycommand expired");
    state.velocity = 0;
    velocityCommandTime.reset();
    printState();
  }
  if (turnCommandTime.isExpired())
  {
    Serial.println("turncitycommand expired");
    state.turnAmount = 0;
    state.turnDirection = TURN_NONE;
    turnCommandTime.reset();
    printState();
  }
}

String command;

void printState()
{
  Serial.println("velocity:");
  Serial.println(state.velocity);
  Serial.println("turn direction:");
  Serial.println(state.turnDirection);
  Serial.println("turn amount:");
  Serial.println(state.turnAmount);
}

void loop()
{
  drive(state.velocity, state.turnDirection, state.turnAmount);
  resetIfExpired();

  readCommand(command);
  if (command.length() > 0)
  {
    Serial.println("command from readCommand:");
    Serial.println(command);
    handleCommand(command);
    command.remove(0);
    printState();
  }
}