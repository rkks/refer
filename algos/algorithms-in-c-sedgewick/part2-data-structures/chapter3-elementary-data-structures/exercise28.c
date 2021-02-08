/* Exercise 3.28: When building the list, Program 3.9 sets twice as many link
 * values as it needs to because it maintains a circular list after each node
 * is inserted. Modify the program to build the circular list without doing
 * this extra work.
 */
#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

int main(int argc, char *argv[]) {
  int i, n = atoi(argv[1]), m = atoi(argv[2]);
  link t = malloc(sizeof *t), x = t;

  t->item = 1;
  for (i = 2; i <= n; ++i) {
    x = (x->next = malloc(sizeof *x));
    x->item = i;
  }
  x->next = t;
  while (x != x->next) {
    for (i = 1; i < m; ++i)
      x = x->next;
    printf("removed: %d\n", x->next->item);
    x->next = x->next->next;
    --n;
  }
  printf("%d\n", x->item);
  return 0;
}
