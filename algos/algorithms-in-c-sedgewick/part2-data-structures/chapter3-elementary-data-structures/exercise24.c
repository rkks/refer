/* Exercise 3.24: Write a function that returns the number of nodes on a
 * circular list, given a pointer to one of the nodes on the list.
 */
#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

int count_links(link root);

int main(int argc, char *argv[]) {
  int i, N = atoi(argv[1]), M = atoi(argv[2]);
  link t = malloc(sizeof *t), x = t;

  t->item = 1;
  t->next = t;
  for (i = 2; i <= N; ++i) {
    x = (x->next = malloc(sizeof *x));
    x->item = i;
    x->next = t;
  }
  while (x != x->next) {
    for (i = 1; i < M; ++i)
      x = x->next;
    printf("removed: %d\n", x->next->item);
    printf("remained count: %d\n", count_links(x));
    x->next = x->next->next;
  }
  printf("%d\n", x->item);
  return 0;
}
