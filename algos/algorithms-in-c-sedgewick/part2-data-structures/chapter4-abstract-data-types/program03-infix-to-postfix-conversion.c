/* Program 4-3: Infix-to-postfix conversion
 * This program is another example of a pushdown-stack client. In this
 * case, the stack contains characters-we assume that Item is defined to
 * be char (that is, we use a different Item.h file than for Program 4.2).
 * To convert (A+B) to the postfix form AB+, we ignore the left parenthesis,
 * convert A to postfix, save the + on the stack, convert B to postfix, then,
 * on encountering the right parenthesis, pop the stack and output the +.
 */
#include <stdio.h>
#include <string.h>
#include "ItemChar.h"
#include "STACK.h"

int main(int argc, char *argv[]) {
  char *a = argv[1];
  int i, N = strlen(a);
  STACKinit(N);

  for (i = 0; i < N; ++i) {
    if (a[i] == ')')
      printf("%c ", STACKpop());
    if ((a[i] == '+') || (a[i] == '*'))
      STACKpush(a[i]);
    if ((a[i] >= '0') && (a[i] <= '9'))
      printf("%c ", a[i]);
  }
  printf("\n");
  return 0;
}
