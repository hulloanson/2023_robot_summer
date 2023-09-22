#ifndef wifiSetup_h
#define wifiSetup_h

#include "RobotStats.h";

void updateDisplay(RobotStats *rStat);

void sendMessage(String msg);

int32_t getTargetVelocity();

unsigned long getLastCommandTime();

void setupWifi();

void setupWebsocketServer();

void runServerLoop();

#endif
