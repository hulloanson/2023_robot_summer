#include <Arduino.h>

int PWMA_PIN = 32;
int PWMB_PIN = 26;

void setupSerial()
{
  Serial.begin(115200);
  // while (!Serial.available())
  //   ;
}

void setupPins()
{
  pinMode(PWMA_PIN, OUTPUT);
  pinMode(PWMB_PIN, OUTPUT);
}

void turnMotorPower(int onOff)
{
  digitalWrite(PWMA_PIN, onOff);
  digitalWrite(PWMB_PIN, onOff);
}

void setup()
{
  setupSerial();
  turnMotorPower(0);
}

void readCommand()
{
  if (Serial.available() == 0)
  {
    return;
  }
  String input = Serial.readStringUntil('\n');
  input.trim();
  if (input.length() < 2)
  {
    Serial.println("Command too short.");
    return;
  }
  char command = input[0];
  String data = input.substring(1);
  if (command == 'P')
  {
    // power
    if (data[0] == '1')
    {
      Serial.println("Turning on motor");
      turnMotorPower(1);
    }
    else if (data[0] == '0')
    {
      Serial.println("Turning off motor");
      turnMotorPower(0);
    }
  }
}

void loop()
{
  readCommand();
}