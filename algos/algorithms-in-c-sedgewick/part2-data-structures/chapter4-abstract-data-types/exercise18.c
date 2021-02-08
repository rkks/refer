/* Exercise 4.18: Give the contents of s[0], ..., s[4] after the execution of
 * the operations illustrated in Figure 4.1, using Program 4.4.
 */
#include <stdio.h>
#include "ItemChar.h"
#include "STACK.h"
#include "STACKARRAY.h"
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
  }

  printf("s[0] = %c\n", STACKget(0));
  printf("s[1] = %c\n", STACKget(1));
  printf("s[2] = %c\n", STACKget(2));
  printf("s[3] = %c\n", STACKget(3));
  printf("s[4] = %c\n", STACKget(4));
  return 0;
}
