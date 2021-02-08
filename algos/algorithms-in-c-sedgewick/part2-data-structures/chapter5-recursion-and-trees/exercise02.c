/* Exercise 5.2: Modify Program 5.1 to compute N! mod M, such that overflow is
 * no longer an issue. Try running your program for M = 997 and
 * N = 10^3, 10^4, 10^5, and 10^6, to get an indication of how your programming
 * system handles deeply nested recursive calls.
 */
#include <stdio.h>
#include <stdlib.h>

int factorial(int N, int M) {
  if (N >= M)
    return 0;
  if (N == 0)
    return 1;
  return N*factorial(N-1, M)%M;
}

int main(int argc, char *argv[]) {
  int n = atoi(argv[1]), m = atoi(argv[2]);

  printf("input: N = %d, M = %d\n", n, m);
  printf("result: %d\n", factorial(n, m));
  return 0;
}
