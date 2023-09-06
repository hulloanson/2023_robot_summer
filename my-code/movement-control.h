#ifndef movement_control_h
#define movement_control_h

void setDirection(int dirA, int dirB);

void standby(int shouldStandby);

void drive(int velocity, int turnDirection = 0, int turnAmount = 0);

void setupMovementPins();

#endif