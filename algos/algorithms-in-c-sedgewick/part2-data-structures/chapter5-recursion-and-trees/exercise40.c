/* Exercise 5.40: Write a function that uses bottom-up dynamic
 * programming to compute the value of P_N defined by the recurrence
 *
 * P_N = [N/2] + P_[N/2] + P_[N/2], for N >= 1 with P_0 = 0.
 *
 * draw a plot of N versus P_N - N log N/2 for 0 <= N <= 1024.
 */
#include <stdio.h>
#include <math.h>

#define N 1024

void P(const int n, int *storage) {
  int i, i2;

  storage[0] = 0;
  for (i = 1; i <= n; ++i) {
    i2 = i>>1;
    storage[i] = i2 + storage[i2] + storage[i2];
  }
}

int main(const int argc, char *argv[]) {
  int storage[N+1], i;
  double plot_y;

  P(N, storage);

  printf("%4d| %d - %d = %d\n", 0, storage[0], 0, 0);
  for (i = 1; i <= N; ++i) {
    plot_y = storage[i] - i*log2(i)/2;
    printf("%4d| %d - %lf = %lf", i, storage[i], i*log2(i)/2, plot_y);
    /* while (plot_y--) */
    /*   putchar('*'); */
    putchar('\n');
  }
  return 0;
}
