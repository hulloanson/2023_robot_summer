// double *calcPIDOutput(int desiredPoint, double &accL, double &accR, double &prevL, double &prevR);
void velocityFunc(double velo, int channel);
double calcPIDOutput2(int desiredPoint, double input, double &lastError, double &lastInput);