/* Exercise 3.2: Test the random-number generator on your system by generating
 * N random integers between 0 and r-1 with `rand()%r` and computing the
 * average and standard deviation for r = 10, 100, and 1000 and N = 10^3, 10^4,
 * 10^5, and 10^6.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]) {
  int r = atoi(argv[1]), n = atoi(argv[2]), rn, nc = n;
  long sum = 0, squaresum = 0;
  float mean, variance, stddev;

  printf("r = %d, n = %d\n", r, n);
  while (nc--) {
    rn = rand()%r;
    sum += rn;
    squaresum += rn*rn;
  }
  mean = ((float) sum)/n;
  variance = (squaresum - sum*sum/((float)n))/((float)n-1);
  stddev = sqrt(variance);
  
  printf("mean = %f\nstandard deviation = %f\n", mean, stddev);
  return 0;
}
