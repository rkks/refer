/* Program 3.7: Coin-flipping simulation
 * If we flip a coin N times, we expect to get N/2 heads, but
 * could get anywhere from 0 to N heads. This program runs the
 * experiment M times, taking both N and M from the command line.
 * It uses an array f to keep track of the frequency of occurrence
 * of the outcome "i heads" for 0 <= i <= N, then prints out a
 * histogram of the result of the experiments, with one asterisk
 * for each 10 occurrences.
 *
 * The operation on which this program is based -- indexing an
 * array with a computed value -- is critical to the efficiency of
 * many computational prcedures.
 */
#include <stdio.h>
#include <stdlib.h>

int heads() {
  return rand() < RAND_MAX/2;
}

int main(int argc, char *argv[]) {
  int i, j, cnt;
  int N = atoi(argv[1]), M = atoi(argv[2]);
  int *f = malloc((N+1)*sizeof(int));

  for (j = 0; j <= N; ++j)
    f[j] = 0;
  for (i = 0; i < M; ++i, ++f[cnt])
    for (cnt = 0, j = 0; j <= N; ++j)
      if (heads())
        ++cnt;
  for (j = 0; j <= N; ++j) {
    printf("%2d ", j);
    for (i = 0; i < f[j]; i += 10)
      printf("*");
    printf("\n");
  }
  return 0;
}
