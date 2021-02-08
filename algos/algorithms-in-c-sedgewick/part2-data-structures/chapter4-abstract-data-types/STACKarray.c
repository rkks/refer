/* Program 4.4: Array implementation of a pushdown stack
 * When there are N items in the stack, this implementation keeps them in
 * s [0], ... , s [N-1]; in order from least recently inserted to most recently
 * inserted. The top of the stack (the position where the next item to
 * be pushed will go) is s nn. The client program passes the maximum
 * number of items expected on the stack as the argument to STACKinit,
 * which allocates an array of that size, but this code does not check for
 * errors such as pushing onto a full stack (or popping an empty one).
 *
 * Property 4.1: We can implement the push and pop operations for
 * the pushdown stack ADT in constant time, using either arrays or
 * linked lists.
 */
#include <stdio.h>
#include <stdlib.h>
#include "ItemChar.h"
#include "STACK.h"
static Item *s;
static int N;
static int MAXN;  // for Exercise 4.20

void STACKerror() {  // for Exercise 4.20
  printf("STACKERROR occurred.\n");
  exit(EXIT_FAILURE);
}

void STACKinit(int maxN) {
  s = malloc(maxN*sizeof(Item));
  N = 0;
  MAXN = maxN;  // for Exercise 4.20
}

int STACKempty() {
  return N == 0;
}

void STACKpush(Item item) {
  if (N == MAXN)  // for Exercise 4.20
    STACKerror();
  s[N++] = item;
}

Item STACKpop() {
  if (STACKempty())  // for Exercise 4.20
    STACKerror();
  return s[--N];
}

/* Exercise 4.1 */
Item STACKget(int i) {
  return s[i];
}

/* Exercise 4.2: Count */
int STACKcount() {
  return N;
}
