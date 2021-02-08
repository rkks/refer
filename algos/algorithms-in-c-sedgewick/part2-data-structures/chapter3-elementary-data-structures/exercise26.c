/* Exercise 3.26: Write a code fragment that, given pointers x and t to two
 * distinct circular lists, inserts the list pointed to by t into the list
 * pointed to by x, at the point following x.
 */
#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"
#define N 10

link connect_lists(link start, link end);

int main(int argc, char *argv[]) {
  int i;
  link t1 = malloc(sizeof *t1), x1 = t1;
  link t2 = malloc(sizeof *t2), x2 = t2;

  t1->item = 1;
  t1->next = t1;
  for (i = 2; i <= N; ++i) {
    x1 = (x1->next = malloc(sizeof *x1));
    x1->item = i;
    x1->next = t1;
  }
  t2->item = i;
  t2->next = t2;
  for (++i; i<= 2*N; ++i) {
    x2 = (x2->next = malloc(sizeof *x2));
    x2->item = i;
    x2->next = t2;
  }

  printf("=== List1 ===\n");
  x1 = t1;
  while (t1 != x1->next) {
    printf("%d ", x1->item);
    x1 = x1->next;
  }
  printf("%d\n", x1->item);

  printf("=== List2 ===\n");
  x2 = t2;
  while (t2 != x2->next) {
    printf("%d ", x2->item);
    x2 = x2->next;
  }
  printf("%d\n", x2->item);

  t1 = connect_lists(t1, t2);
  x1 = t1;
  printf("=== List1 + List2 ===\n");
  while (t1 != x1->next) {
    printf("%d ", x1->item);
    x1 = x1->next;
  }
  printf("%d\n", x1->item);
  return 0;
}
