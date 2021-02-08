/* Exercise 3.11 */
#include <stdio.h>

int main() {
  int i, a[99];

  for (i = 0; i < 99; ++i)
    a[i] = 98 - i;
  for (i = 0; i < 99; ++i) {
    a[i] = a[a[i]];
    printf("%d ", a[i]);
  }
  printf("\n");
  return 0;
}
