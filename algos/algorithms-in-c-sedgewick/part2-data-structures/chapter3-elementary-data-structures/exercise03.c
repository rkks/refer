/* Exercise 3.3: Test the random-number generator on your system by generating N
 * random numbers of type `double` between 0 and 1, transforming them to
 * integers between 0 and r-1 by multiplying by r and truncating the result,
 * and computing the average and standard deviation for r = 10, 100, and 1000,
 * and N = 10^3, 10^4, 10^5 and 10^6.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define DRAND() (rand()/((double)RAND_MAX))

int main(int argc, char *argv[]) {
  int r = atoi(argv[1]), n = atoi(argv[2]), rni, nc = n;
  double rn;
  long sum = 0, squaresum = 0;
  float mean, variance, stddev;

  printf("r = %d, n = %d\n", r, n);
  while (nc--) {
    rn = DRAND();
    rni = (int) (rn*r);
    sum += rni;
    squaresum += rni*rni;
  }
  mean = ((float) sum)/n;
  variance = (squaresum - sum*sum/((float)n))/((float)n-1);
  stddev = sqrt(variance);

  printf("mean = %f\nstandard deviation = %f\n", mean, stddev);
  return 0;
}
