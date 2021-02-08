/* Program 3.14: Implementation of list-processing interface
 * This program gives implementations of the functions declared in
 * Program 3.12, and illustrates a standard approach to allocating
 * memory for fixed-size nodes. We build a free list that is
 * initialized to the maximum number of nodes that our program
 * will use, all linked together. Then, when a client program
 * allocates a node, we remove that node from the free list; when
 * a client program frees a node, we link that node in to the free
 * list.
 *
 * By convention, client program do not refer to list nodes except
 * through function calls, and nodes returned to client programs
 * have self-links. These conventions provide some measure of
 * protection against referencing undefined pointers.
 */
#include <stdlib.h>
#include "list.h"

link freelist;

void initNodes(int N) {
  int i;
  freelist = malloc((N+1)*(sizeof *freelist));

  for (i = 0; i < N+1; ++i)
    freelist[i].next = &freelist[i+1];
  freelist[N].next = NULL;
}

link newNode(int i) {
  link x = deleteNext(freelist);
  x->item = i;
  x->next = x;
  return x;
}

void freeNode(link x) {
  insertNext(freelist, x);
}

void insertNext(link x, link t) {
  t->next = x->next;
  x->next = t;
}

link deleteNext(link x) {
  link t = x->next;
  x->next = t->next;
  return t;
}

link Next(link x) {
  return x->next;
}

int Item(link x) {
  return x->item;
}
