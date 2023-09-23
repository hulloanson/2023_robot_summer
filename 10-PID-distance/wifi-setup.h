#ifndef wifiSetup_h
#define wifiSetup_h

#include "RobotStats.h";
#include "UserCommand.h";

void updateDisplay(RobotStats *rStat);

void sendMessage(String msg);

UserCommand *getLastCommand();

void setupWifi();

void setupWebsocketServer();

void runServerLoop();

#endif
