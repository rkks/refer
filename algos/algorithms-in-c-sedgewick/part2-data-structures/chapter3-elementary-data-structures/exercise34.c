/* Exercise 3.34: Write a function that moves the largest item on
 * a given list to be final node on the list.
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "list.h"
#define N 10

void push_max(Node);

int main() {
  Node t, x, start;

  initNodes(N);
  start = newNode(0);
  insertNext(start, (t = newNode(9)));
  insertNext(t, (x = newNode(8)));
  insertNext(x, (t = newNode(7)));
  insertNext(t, (x = newNode(6)));
  insertNext(x, (t = newNode(5)));
  insertNext(t, (x = newNode(4)));
  insertNext(x, (t = newNode(3)));
  insertNext(t, (x = newNode(2)));
  insertNext(x, (t = newNode(1)));
  t->next = NULL;

  printf("=== Original List ===\n");
  for (x = start; x != NULL; x = Next(x))
    printf("%d ", Item(x));
  printf("\n");

  push_max(start);

  printf("=== Max-pushed List ===\n");
  for (x = start; x != NULL; x = Next(x))
    printf("%d ", Item(x));
  printf("\n");

  return 0;
}

void push_max(link x) {
  int max = INT_MIN;
  link t, maxt, last;

  for (t = x; t != NULL; t = t->next) {
    if (t->item > max) {
      max = t->item;
      maxt = t;
    }
    if (t->next == NULL)
      last = t;
  }

  for (t = x; t != NULL; t = t->next)
    if (t->next == maxt)
      deleteNext(t);
  last->next = maxt;
  maxt->next = NULL;
}
