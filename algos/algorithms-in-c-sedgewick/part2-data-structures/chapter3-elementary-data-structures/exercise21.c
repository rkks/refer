/* Exercise 3.21: Modify Program 3.7 to simulate a situation where
 * the coin turns up head with probability lambda/N. Run 1000
 * trials for an experiment with 32 flips with p = 1/6 to get
 * output that you can compare with Figure 3.2. This distribution
 * is the classical Poisson distribution.
 */
#include <stdio.h>
#include <stdlib.h>

int heads(int lambda, int N) {
  return rand() < RAND_MAX/N*lambda;
}

int main(int argc, char *argv[]) {
  int i, j, cnt;
  int N = atoi(argv[1]), M = atoi(argv[2]),
      lambda = atoi(argv[3]);
  int *f = malloc((N+1)*sizeof(int));

  for (j = 0; j <= N; ++j)
    f[j] = 0;
  for (i = 0; i < M; ++i, ++f[cnt])
    for (cnt = 0, j = 0; j <= N; ++j)
      if (heads(lambda, N))
        ++cnt;
  for (j = 0; j <= N; ++j) {
    printf("%2d ", j);
    for (i = 0; i < f[j]; i += 10)
      printf("*");
    printf("\n");
  }
  return 0;
}
