#include "PID_v1.h"

double kp0 = .5;
double ki0 = 0.2;
double kd0 = 0.2;

double setPointA, inputA, outputA;
double setPointB, inputB, outputB;

PID pidA(&inputA, &outputA, &setPointA, kp0, ki0, kd0, DIRECT);
PID pidB(&inputB, &outputB, &setPointB, kp0, ki0, kd0, DIRECT);

void setupPids()
{
  pidA.SetOutputLimits(-1000.0, 1000.0);
  pidB.SetOutputLimits(-1000.0, 1000.0);
}

void setPidMode(bool automatic)
{
  int mode = automatic ? AUTOMATIC : MANUAL;
  pidA.SetMode(mode);
  pidB.SetMode(mode);
}

void setPidSetPoints(int leftSetPoint, int rightSetPoint)
{
  setPointA = (double)leftSetPoint;
  setPointB = (double)rightSetPoint;
}

void setPidInputs(int leftInput, int rightInput)
{
  inputA = (double)leftInput;
  inputB = (double)rightInput;
}

void computePids(int *leftOutput, int *rightOutput)
{
  pidA.Compute();
  pidB.Compute();
  *leftOutput = (int)outputA;
  *rightOutput = (int)outputB;
}