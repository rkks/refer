/* Program 3.11: List insertion sort
 * This code generates N random integers between 0 and 999, builds
 * a linked list with one number per node (first `for` loop), and
 * then rearranges the nodes so that the numbers appear in order
 * when we traverse the list (second `for` loop). To accomplish
 * the sort, we maintain two lists, an input (unsorted) list and
 * an output (sorted) list. On each iteration of the loop, we
 * remove a node from the input and insert it into position in the
 * output. The code is simplified by the use of head nodes for
 * each list, that contain the links to the first nodes on the
 * lists. For example, without the head node, the case where the
 * node to be inserted into the output list goes at the beginning
 * would involve extra code.
 */
#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"
#define N 100

int main() {
  int i;
  struct node heada, headb;
  link t, u, x, a = &heada, b;
  
  for (i = 0, t = a; i < N; ++i) {
    t->next = malloc(sizeof *t);
    t = t->next;
    t->next = NULL;
    t->item = rand() % 1000;
  }
  printf("=== Original List ===\n");
  for (t = a->next; t != NULL; t = t->next)
    printf("%d ", t->item);
  printf("\n");

  b = &headb;
  b->next = NULL;
  for (t = a->next; t != NULL; t = u) {
    u = t->next;
    for (x = b; x->next != NULL; x = x->next)
      if (x->next->item > t->item)
        break;
    t->next = x->next;
    x->next = t;
  }
  printf("=== Sorted List ===\n");
  for (t = b->next; t != NULL; t = t->next)
    printf("%d ", t->item);
  printf("\n");

  return 0;
}
