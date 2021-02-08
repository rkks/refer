/* Exercise 5.6: Give the figure corresponding to Figure 5.2 for the
 * result of running Euclid's algorithm for the inputs 89 and 55.
 */
#include <stdio.h>
#include <stdlib.h>

static int depth;

int gcd_with_print(const int, const int);

int main(int argc, char *argv[]) {
  const int m = atoi(argv[1]),
            n = atoi(argv[2]);
  depth = 0;
  gcd_with_print(m, n);
  return 0;
}

int gcd_with_print(int m, int n) {
  int i;
  for (i = 0; i < depth; ++i)
    printf("  ");
  printf("gcd(%d, %d)\n", m, n);
  ++depth;

  if (n == 0)
    return m;
  return gcd_with_print(n, m%n);
}
