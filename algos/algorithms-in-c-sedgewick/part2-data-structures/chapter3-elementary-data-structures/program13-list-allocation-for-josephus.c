/* Program 3.13: List allocation for the Josephus problem
 * This program for the Josephus problem is an example of a client
 * program utilizing the list-processing primitives declared in
 * Program 3.12 and implemented in Program 3.14.
 */
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

int main(int argc, char *argv[]) {
  int i, N = atoi(argv[1]), M = atoi(argv[2]);
  Node t, x;

  initNodes(N);
  for (i = 2, x = newNode(1); i <= N; ++i) {
    t = newNode(i);
    insertNext(x, t);
    x = t;
  }
  while (x != Next(x)) {
    for (i = 1; i < M; ++i)
      x = Next(x);
    freeNode(deleteNext(x));
  }
  printf("%d\n", Item(x));
  
  return 0;
}
