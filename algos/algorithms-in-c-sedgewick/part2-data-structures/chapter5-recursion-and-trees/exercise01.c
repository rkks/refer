/* Exercise 5.1: Write a recursive program to compute lg(N!).
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float logfactorial(int n) {
  if (n < 0)
    return -1.0;
  if (n == 1)
    return 0.0;
  return logf((float) n) + logfactorial(n-1);
}

int main(int argc, char *argv[]) {
  int input = atoi(argv[1]);

  printf("input: %d\n", input);
  printf("log(%d!) = %f\n", input, logfactorial(input));
  return 0;
}
