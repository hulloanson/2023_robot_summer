
void setupPids();

void setPidMode(bool automatic);

void setPidSetPoints(int leftSetPoint, int rightSetPoint);

void setPidInputs(int leftInput, int rightInput);

void computePids(int *leftOutput, int *rightOutput);