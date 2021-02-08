/* Exercise 4.38: Provide an implementation for your deque
 * interface (Exercise 4.37) that uses an array for the
 * underlying data structure.
 */
#include <stdio.h>
#include <stdlib.h>
#include "ItemChar.h"
static Item *deq;
static int N, head, tail, cnt;

void DEQUEinit(const int maxN) {
  N = maxN;
  deq = malloc(N*sizeof(Item));
  head = N/2;
  tail = head+1;
  cnt = 0;
}

void DEQUEdestroy() {
  free(deq);
}

int DEQUEempty() {
  return cnt == 0;
}

void DEQUEprepend(Item item) {
  deq[head--] = item;
  head %= N;
  ++cnt;
}

// same as QUEUEget
Item DEQUEget() {
  Item item = deq[++head];
  head %= N;
  --cnt;
  return item;
}

// same as QUEUEput
void DEQUEappend(Item item) {
  deq[tail++] = item;
  tail %= N;
  ++cnt;
}

Item DEQUEpop() {
  Item item = deq[--tail];
  tail %= N;
  --cnt;
  return item;
}

void DQdump(const char inputc, const char outputc) {
  int i;
  printf("%c %c|", inputc, outputc);
  for (i = 0; i <= N; ++i)
    printf(" %c", deq[i]);
  printf("\n");
}
