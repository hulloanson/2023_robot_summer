#include <Arduino.h>

int interpolate(int x0, int y0, int x1, int y1, int x)
{
  return (int)((double)(y1 - y0) / (x1 - x0) * (x - x0) + y0 + .5);
  // Serial.printf("(%d, %d) (%d, %d) => (%d, %d)\n", x0, y0, x1, y1, x, y);
  // return y;
}