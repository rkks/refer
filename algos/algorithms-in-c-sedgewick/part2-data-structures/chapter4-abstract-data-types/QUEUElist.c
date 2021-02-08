/* Program 4.10: FIFO queue linked-list implementation
 * The difference between a FIFO queue and a pushdown stack (Program 4.5) is
 * that new items are inserted at the end, rather than the beginning.
 *
 * Accordingly, this program keeps a pointer tail to the last node of
 * the list, so that the function QUEUEput can add a new node by linking that
 * node to the node referenced by tail and then updating tail to point
 * to the new node. The functions QUEUEget, QUEUEinit, and QUEUEempty
 * are all identical to their counterparts for the linked-list pushdown-stack
 * implementation of Program 4.5.
 *
 * Property 4.2: We can implement the get and put operations for the FIFO queue
 * ADT in constant time, using either arrays or linked lists.
 */
#include <stdlib.h>
#include "ItemChar.h"
#include "QUEUE.h"
typedef struct QUEUEnode* link;
struct QUEUEnode {
  Item item;
  link next;
};
static link head, tail;

link NEW(Item item, link next) {
  link x = malloc(sizeof *x);
  x->item = item;
  x->next = next;
  return x;
}

void QUEUEinit(int maxN) {
  head = NULL;
}

int QUEUEempty() {
  return head == NULL;
}

void QUEUEput(Item item) {
  if (head == NULL) {
    head = (tail = NEW(item, head));
    return;
  }
  tail->next = NEW(item, tail->next);
  tail = tail->next;
}

Item QUEUEget() {
  Item item = head->item;
  link t = head->next;
  free(head);
  head = t;
  return item;
}
