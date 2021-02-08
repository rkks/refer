/* Program 4.11: FIFO queue array implementation
 * The contents of the queue are all the elements in the array between head
 * and tail, taking into account the wraparound back to 0 when the end of
 * the array is encountered. If head and tail are equal, then we consider
 * the queue to be empty; but if put would make them equal, then we
 * consider it to be full. As usual, we do not check such error conditions,
 * but we make the size of the array 1 greater than the maximum number
 * of elements that the client expects to see in the queue, so that we could
 * augment this program to make such checks.
 *
 * Property 4.2: We can implement the get and put operations for the FIFO queue
 * ADT in constant time, using either arrays or linked lists.
 */
#include <stdlib.h>
#include "ItemChar.h"
static Item *q;
static int N, head, tail;

void QUEUEinit(const int maxN) {
  N = maxN+1;
  q = malloc(N*sizeof(Item));
  head = N;
  tail = 0;
}

int QUEUEempty() {
  return head%N == tail;
}

void QUEUEput(Item item) {
  q[tail++] = item;
  tail %= N;
}

Item QUEUEget() {
  head %= N;
  return q[head++];
}

/* for Exercise 4.30 */
Item QUEUEget_by_index(int i) {
  return q[i];
}
