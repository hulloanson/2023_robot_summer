#include <Arduino.h>

#include "global-var.h"
#include "encoder-control.h"

double kP = 0;
double kI = 0;
double kD = 0;

double calcPIDOutput2(int desiredPoint, double input, double &lastError, double &lastInput)
{

    double error = desiredPoint - input;

    double dInput = input - lastInput;

    double iError = lastError + error;

    double output = kP * error + kI * iError + kD * dInput;

    output > 255 ? output = 255 : 0;

    lastInput = input;

    lastError = iError;

    return output;
}

/*
double *calcPIDOutput(int desiredPoint, double &accL, double &accR, double &prevL, double &prevR)
{
    double distTravelledL = calcEncoder()[0];
    double distTravelledR = calcEncoder()[1];

    double errorL = desiredPoint - distTravelledL;
    double errorR = desiredPoint - distTravelledR;

    accL += errorL;
    accR += errorR;

    double outputL = kP * errorL + kI * accL + kD * prevL;
    double outputR = kP * errorR + kI * accR + kD * prevR;

    prevL = distTravelledL;
    prevR = distTravelledR;

    double outputValues[2] = {outputL, outputR};
    return outputValues;
}
*/