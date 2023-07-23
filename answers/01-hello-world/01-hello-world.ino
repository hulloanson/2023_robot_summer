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

// define variable number, of type int, as 1
int number = 1;

// define variable message, of type String, as "Hello world!"
String message = "Hello world!";

String anotherMessage = "How are you world?";

void loop()
{
  Serial.println("Message:");
  Serial.println(message);
  Serial.println(number);
  Serial.println(anotherMessage);
  delay(1000);
}