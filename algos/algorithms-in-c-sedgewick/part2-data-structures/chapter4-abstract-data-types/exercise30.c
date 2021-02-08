/* Exercise 4.30: Give the contents of q[0], ..., q[4] after the execution of
 * the operations illustrated in Figure 4.6, using Program 4.11. Assume that
 * maxN is 10, as in Figure 4.8.
 */
#include <stdio.h>
#include "ItemChar.h"
#include "QUEUE.h"
#define N 10

Item QUEUEget_by_index(int);

int main() {
  Item c;

  QUEUEinit(N);
  while ((c = getchar()) != EOF) {
    if (c == '*')
      QUEUEget();
    else if (c == '\n')
      continue;
    else
      QUEUEput(c);
  }

  printf("q[0] = %c\n", QUEUEget_by_index(0));
  printf("q[1] = %c\n", QUEUEget_by_index(1));
  printf("q[2] = %c\n", QUEUEget_by_index(2));
  printf("q[3] = %c\n", QUEUEget_by_index(3));
  printf("q[4] = %c\n", QUEUEget_by_index(4));
  return 0;
}
