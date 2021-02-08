/* Exercise 4.20: Modify the array-based pushdown-stack implementation in the
 * text (Program 4.4) to call a function `STACKerror` if the client attempts to
 * `pop` when the stack is empty or `push` when the stack is full.
 */
/* Exercise 4.21: Modify the linked-list-based pushdown-stack implementation in
 * the text (Program 4.5) to call a function `STACKerror` if the client
 * attempts `pop` when the stack is empty or if there is no memory available
 * from `malloc` for a `push`.
 */
#include <stdio.h>
#include "ItemChar.h"
#include "STACK.h"

int main() {
  STACKinit(10);
  STACKpop();
  printf("This print is never called.\n");
  return 0;
}
