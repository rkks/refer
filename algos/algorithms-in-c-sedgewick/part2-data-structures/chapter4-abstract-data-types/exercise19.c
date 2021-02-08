/* Exercise 4.19: Suppose that you change the pushdown-stack interface to
 * replace `test if empty` by `count`, which should return the number of items
 * currently in the data structure. Provide implementations for `count` for
 * the array representation (Program 4.4) and the linked-list representation
 * (Program 4.5).
 */
#include <stdio.h>
#include "ItemChar.h"
#include "STACK.h"
#define N 20

int main() {
  Item c;

  STACKinit(N);
  while ((c = getchar()) != EOF) {
    if (c == '*')
      STACKpop();
    else if (c == '\n')
      continue;
    else
      STACKpush(c);
    printf("Stack count: %d\n", STACKcount());
  }
  return 0;
}
