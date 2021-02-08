/* Exercise 3.25: Write a code fragment that determines the number of nodes 
 * that are between the nodes referenced by two given pointers x and t to nodes
 * on a circular list.
 */
#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

int count_links_between(link start, link end);

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
    printf("remained count: %d\n", count_links_between(x, x));
    x->next = x->next->next;
  }
  printf("%d\n", x->item);
  return 0;
}
