/* Program 5.3: Euclid's algorithm
 * One of the oldest-known algorithms, dating back over 2000 years, is this
 * recursive method for finding greatest common divisors of two integers.
 */
#include <stdio.h>
#include <stdlib.h>

int gcd(int m, int n) {
  if (n == 0)
    return m;
  return gcd(n, m%n);
}

int main(int argc, char *argv[]) {
  int m = atoi(argv[1]), n = atoi(argv[2]),
      result = gcd(m, n);
  printf("GCD of %d and %d: %d\n", m, n, result);
  return 0;
}
