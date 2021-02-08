/* Exercise 5.41: Write a function that uses top-down dynamic
 * programming to solve Exercise 5.40.
 */
#include <stdio.h>
#include <math.h>

#define N 1024

int P(const int n, int *storage) {
  int n2 = n>>1;
  if (n == 0)
    return storage[n] = n;
  if (storage[n] >= 0)
    return storage[n];
  return storage[n] = n2 + (P(n2, storage)<<1);
}

int main(const int argc, char *argv[]) {
  int storage[N+1] = {-1}, i;
  double plot_y;

  for (i = 0; i <= N; ++i)
    storage[i] = -1;

  printf("%4d| %d - %d = %d\n", 0, P(0, storage), 0, 0);
  for (i = 1; i <= N; ++i) {
    plot_y = storage[i] - i*log2(i)/2;
    printf("%4d| %d - %lf = %lf", i, P(i, storage), i*log2(i)/2, plot_y);
    /* while (plot_y--) */
    /*   putchar('*'); */
    putchar('\n');
  }
  return 0;
}
