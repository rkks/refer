/* Exercise 3.61: Implement a pointer version of Program 3.15.
 */
#include <stdio.h>
#define N 10000

int main(int argc, char *argv[]) {
  int t;
  char a[N], *ap, *p = argv[1], *pp;

  for (ap = a; ap-a < N-1; *ap = t, ++ap)
    if ((t = getchar()) == EOF)
      break;
  *ap = 0;

  for (ap = a; *ap != 0; ++ap) {
    for (pp = p; *pp != 0; ++pp)
      if (*(ap+(pp-p)) != *pp)
        break;
    if (*pp == 0)
      printf("%ld ", ap-a);
  }
  printf("\n");
  return 0;
}
