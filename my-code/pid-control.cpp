#include <Arduino.h>

#include "global-var.h"
#include "encoder-control.h"

double kP = 0;
double kI = 0;
double kD = 0;

void calcPIDOutput(int desiredPoint, double &accL, double &accR, double &prevL, double &prevR)
{
    double distTravelledL = calcEncoder()[0];
    double distTravelledR = calcEncoder()[1];

    double errorL = desiredPoint - distTravelledL;
    double errorR = desiredPoint - distTravelledR;

    accL += errorL;
    accR += errorR;

    double outputL = kP * errorR + kI * accL + kD * prevL;

    prevL = distTravelledL;
    prevR = distTravelledR;
}

void move(int desiredDistance)
{
    bool moving = true;

    double accL;
    double accR;
    double prevL;
    double prevR;

    while (move)
    {
        calcPIDOutput(desiredDistance, accL, accR, prevL, prevR);
    }
}
