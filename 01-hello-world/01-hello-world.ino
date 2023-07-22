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
int number2 = 733;

// TODO: describe the message variable
String message = "Hello world!";
String message2 = "Among us is the best game in the world";

// TODO: define another message and print it

void loop()
{
  Serial.println("Message:");
  Serial.println(message);
  Serial.println(number);

  Serial.println("Message 2:");
  Serial.println(message2);
  Serial.println(number2);
  delay(1000);
}