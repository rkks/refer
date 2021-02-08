/* Program 4.5: Linked-list implementation of a pushdown stack
 * This code implements the stack ADT as illustrated in
 * 4.5. It uses
 * an auxiliary function NEW to allocate the memory for a node, set its fields
 * from the function arguments, and return a link to the node.
 *
 * Property 4.1: We can implement the push and pop operations for
 * the pushdown stack ADT in constant time, using either arrays or
 * linked lists.
 */
#include <stdio.h>
#include <stdlib.h>
#include "ItemChar.h"
typedef struct STACKnode* link;
struct STACKnode {
  Item item;
  link next;
};
static link head;
static int N;

void STACKerror() {  // for Exercise 4.21
  printf("STACKERROR occurred.\n");
  exit(EXIT_FAILURE);
}

link NEW(Item item, link next) {
  link x = malloc(sizeof *x);
  if (x == NULL)  // for Exercise 4.21
    STACKerror();
  
  x->item = item;
  x->next = next;
  return x;
}

void STACKinit(int maxN) {
  head = NULL;
  N = 0;
}

int STACKempty() {
  return head == NULL;
}

void STACKpush(Item item) {
  head = NEW(item, head);
  ++N;
}

Item STACKpop() {
  if (STACKempty())  // for Exercise 4.21
    STACKerror();

  Item item = head->item;
  link t = head->next;
  free(head);
  head = t;
  --N;
  return item;
}

/* Exercise 4.2: count */
int STACKcount() {
  return N;
}
