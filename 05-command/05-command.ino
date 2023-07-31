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
    if (velocity > 0)
    {
      cwOrCCW(1, 1);

      analogWrite(PWMA_PIN, velocity * 1.02);
      analogWrite(PWMB_PIN, velocity);
    }
    else if (velocity < 0)
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
  while (true)
  {
    int message = Serial.read();
    if (message == -1)
    {
      break;
    }
    if (message == 10)
    {
      break;
    }
    command += (char)message;
  }
  return command;
}

/// @brief
/// @param input
///         it takes 2 commands
///         1. V: V100 drive the robot forward, V-50 drive it backward (but slower), etc
void handleCommand(String input)
{
  Serial.println("command from readCommand:");
  Serial.println(input);
  if (input[0] == 'V')
  {
    int velocity = input.substring(1).toInt();
    Serial.println("Velocity is " + velocity);
    drive(velocity, TURN_NONE, 0);
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
  /*
  String whatToDo = readCommand1();
  Serial.println("Command is " + whatToDo);
*/

  String command = readCommand2();
  if (command.length() > 0)
  {
    handleCommand(command);
  }
}