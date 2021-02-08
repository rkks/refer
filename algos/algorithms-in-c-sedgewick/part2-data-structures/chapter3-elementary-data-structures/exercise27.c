/* Exercise 3.27: Given pointers x and t to nodes on a circular
 * list, write a code fragment that moves the node following t to
 * the position following the node following x on the list.
 */

#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"
#define N 10

void move_node(link, link);

int main() {
  int i;
  link t = malloc(sizeof *t), x = t;
  link n1, n2;

  t->item = 1;
  t->next = t;
  for (i = 2; i <= N; ++i) {
    x = (x->next = malloc(sizeof *x));
    x->item = i;
    x->next = t;
    if (i == 3)
      n1 = x;
    else if (i == 7)
      n2 = x;
  }
  printf("=== Initial list ===\n");
  for (x = t; x->next != t; x = x->next)
    printf("%d ", x->item);
  printf("%d\n", x->item);

  move_node(n1, n2);
  printf("=== After move_node(3, 7) ===\n");
  for (x = t; x->next != t; x = x->next)
    printf("%d ", x->item);
  printf("%d\n", x->item);  return 0;
}
