/* Exercise 3.20: Modify Program 3.7 to simulate a situation where
 * the coin turns up head with probability p. Run 1000 trials for
 * an experiment with 32 flips with p = 1/6 to get output that you
 * can compare with Figure 3.2.
 */
#include <stdio.h>
#include <stdlib.h>

int heads(float p) {
  return rand() < RAND_MAX*p;
}

int main(int argc, char *argv[]) {
  int i, j, cnt;
  int N = atoi(argv[1]), M = atoi(argv[2]);
  float p = atof(argv[3]);
  int *f = malloc((N+1)*sizeof(int));

  for (j = 0; j <= N; ++j)
    f[j] = 0;
  for (i = 0; i < M; ++i, ++f[cnt])
    for (cnt = 0, j = 0; j <= N; ++j)
      if (heads(p))
        ++cnt;
  for (j = 0; j <= N; ++j) {
    printf("%2d ", j);
    for (i = 0; i < f[j]; i += 10)
      printf("*");
    printf("\n");
  }
  return 0;
}
