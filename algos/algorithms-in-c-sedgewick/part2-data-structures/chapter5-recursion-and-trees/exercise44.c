/* Exercise 5.44: Write a function that uses bottom-up dynamic
 * programming to compute the value of C_N defined by the recurrence
 *
 * C_N = N + 1/N \sum_{1<=k<=N} (C_{k-1} + C_{N-k}), for N >= 1 with C_0 = 1.
 */
#include <stdio.h>
#include <stdlib.h>

void C(const int n, double *storage) {
  int i, j;
  double partialsum;

  storage[0] = 0;
  for (i = 1; i <= n; ++i) {
    for (j = 1, partialsum = 0.0; j <= i; ++j)
      partialsum += storage[j-1] + storage[i-j];
    storage[i] = i + partialsum/i;
  }
}

int main(const int argc, char *argv[]) {
  int n = atoi(argv[1]), i;
  double *storage = (double *) malloc(sizeof(double)*(n+1));

  C(n, storage);

  for (i = 0; i <= n; ++i)
    printf("%10d| %lf\n", i, storage[i]);
  free(storage);
  return 0;
}
