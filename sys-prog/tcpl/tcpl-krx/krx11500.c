#include <stdio.h>

float FtoC(float f)
{
  float c;
  c = (5.0 / 9.0) * (f - 32.0);
  return c;
}

int main(void)
{
  float fahr, celsius;
  int lower, upper, step;

  lower = 0;
  upper = 300;
  step = 20;

  printf("F     C\n\n");
  fahr = lower;
  while(fahr <= upper)
  {
    celsius = FtoC(fahr);
    printf("%3.0f %6.1f\n", fahr, celsius);
    fahr = fahr + step;
  }
  return 0;
}

