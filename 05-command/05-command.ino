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

void cwOrCCW(int cwA, int cwB)
{
  // TODO: print message about whether we're turning clockwise
  if (cwA)
  {
    // TODO: turn clockwise
    digitalWrite(AIN1_PIN, LOW);
    digitalWrite(AIN2_PIN, HIGH);
  }
  else
  {
    // TODO: turn counter-clockwise
    digitalWrite(AIN1_PIN, HIGH);
    digitalWrite(AIN2_PIN, LOW);
  }

  if (cwB)
  {
    // TODO: turn clockwise
    digitalWrite(BIN1_PIN, LOW);
    digitalWrite(BIN2_PIN, HIGH);
  }
  else
  {
    // TODO: turn counter-clockwise
    digitalWrite(BIN1_PIN, HIGH);
    digitalWrite(BIN2_PIN, LOW);
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
  // TODO: put in your version of drive()
  if (turnDirection == TURN_LEFT)
  {
    cwOrCCW(1, 0);

    analogWrite(PWMA_PIN, velocity * 1.02);
    analogWrite(PWMB_PIN, velocity);
  }
  else if (turnDirection == TURN_RIGHT)
  {
    cwOrCCW(0, 1);

    analogWrite(PWMA_PIN, velocity * 1.02);
    analogWrite(PWMB_PIN, velocity);
  }
  else if (turnDirection == TURN_NONE)
  {
    if (velocity <= 0)
    {
      cwOrCCW(1, 1);

      analogWrite(PWMA_PIN, velocity * 1.02);
      analogWrite(PWMB_PIN, velocity);
    }
    else if (velocity > 0)
    {
      cwOrCCW(0, 0);

      analogWrite(PWMA_PIN, abs(velocity * 1.02));
      analogWrite(PWMB_PIN, abs(velocity));
    }
  }
}

String readCommand1()
{
  // Alvin: only returns when it has a command
  // TODO: read command from Serial byte by byte until you see a '\n' (new line character)
  // line ending have be "LF" in the serial monitor for this to work.

  String command = "";
  while (true)
  {
    int message = Serial.read();
    if (message != -1)
    {
      if (message == 10)
      {
        break;
      }
      command += (char)message;
    }
  }
  return command;
}

String readCommand2()
{
  // Alvin: returns a lot more blank lines
  // TODO: read command from Serial byte by byte until you see a '\n' (new line character)
  // line ending have be "LF" in the serial monitor for this to work.

  String command = "";
  int message;
  while (true)
  {
    message = Serial.read();
    if (message == -1 || message == 10)
      break;

    command += (char)message;
  }

  return command;
}

int velocity = 0;

int turnDirection = TURN_NONE;

int turnAmount = 0;

/// @brief
/// @param input
///         it takes 2 commands
///         1. V: V100 drive the robot forward, V-50 drive it backward (but slower), etc
///         2. T: T0 keep the robot straight, T100 to turn robot all the way to the right, T-100 all the way to the left
void handleCommand(String input)
{
  if (input[0] == 'V')
  {
    velocity = input.substring(1).toInt();
  }
  if (input[0] == 'T' && input[1] == 'D')
  {
    turnDirection = input.substring(2).toInt();
  }
  if (input[0] == 'T' && input[1] == 'A')
  {
    turnAmount = input.substring(2).toInt();
  }
}

void setup()
{
  setupSerial();

  setupPins();

  standby(0);
}

void loop()
{
  String command = readCommand2();
  if (command.length() > 0)
  {
    handleCommand(command);
    if (command == "V0")
    {
      Serial.println("Velocity is 0");
    }
    Serial.println(velocity);

    drive(velocity, turnDirection, turnAmount);
  }
}