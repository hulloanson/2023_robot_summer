#include <Arduino.h>

void setupSerial()
{
  Serial.begin(115200);
}

void setup()
{
  setupSerial();
}

// <- 2 slashes: comment, explain what you want to do!

// define variable number, of type integer, as 1
int number = 1;

// TODO: describe the message variable
String message = "Hello world!";

// TODO: define another message and print it

void loop()
{
  Serial.println("Message:");
  Serial.println(message);
  Serial.println(number);
  delay(1000);
}