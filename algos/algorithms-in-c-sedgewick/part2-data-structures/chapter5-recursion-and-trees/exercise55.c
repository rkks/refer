/* Exercise 5.55: Write a program that uses top-down dynamic
 * programming to compute the binomial coefficient (N,k), based on the
 * recursive rule
 *
 * (N,k) = (N-1,k) + (N-1,k-1)
 *
 * with (N,0) = (N,N) = 1.
 */
#include <stdio.h>
#include <stdlib.h>

int bcoeff(const int n, const int k, int **storage) {
  if (storage[n][k] > 0)
    return storage[n][k];
  if (n == 1) {
    storage[n][0] = 1;
    storage[n][1] = 1;
    return 1;
  }
  if (k == 0 || k == n)
    return 1;
  return (storage[n][k] = bcoeff(n-1, k, storage) + bcoeff(n-1, k-1, storage));
}

int main(const int argc, char *argv[]) {
  int n = atoi(argv[1]), k = atoi(argv[2]), i,
      **storage = (int **) malloc(sizeof(int *)*(n+1));

  printf("%d C %d = ", n, k);
  for (i = 1; i <= n; ++i)
    storage[i] = (int *) malloc(sizeof(int)*(i+1));

  printf("%d\n", bcoeff(n, k, storage));

  free(storage);
  return 0;
}
