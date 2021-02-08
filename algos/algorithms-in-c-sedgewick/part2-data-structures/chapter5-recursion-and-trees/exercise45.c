/* Exercise 5.45: Write a function that uses top-down dynamic
 * programming to solve Exercise 5.44.
 */
#include <stdio.h>
#include <stdlib.h>

double C(const int n, double *storage) {
  int j;
  double sum;

  if (n == 0)
    return storage[0] = 0;
  if (storage[n] >= 0)
    return storage[n];

  for (j = 1, sum = 0.0; j <= n; ++j)
    sum += C(j-1, storage) + C(n-j, storage);
  return (storage[n] = n + sum/n);
}

int main(const int argc, char *argv[]) {
  int n = atoi(argv[1]), i;
  double *storage = (double *) malloc(sizeof(double)*(n+1));

  for (i = 0; i <= n; ++i)
    storage[i] = -1;

  for (i = 0; i <= n; ++i)
    printf("%10d| %lf\n", i, C(i, storage));
  free(storage);
  return 0;
}
