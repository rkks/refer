/* Program 4.2: Postfix-expression evaluation
 * This pushdown-stack client reads any postfix expression involving mul­
 * tiplication and addition of integers, then evaluates the expression and
 * prints the computed result.
 * When we encounter operands, we push them on the stack; when
 * we encounter operators, we pop the top two entries from the stack and
 * push the result of applying the operator to them. The order in which
 * the two STACKpopO operations are performed in the expressions in this
 * code is unspecified in C, so the code for noncommutative operators such
 * as subtraction or division would be slightly more complicated.
 * The program assumes that at least one blank follows each integer,
 * but otherwise does not check the legality of the input at all. The final
 * i f statement and the while loop perform a calculation similar to the
 * C atoi function, which converts integers from ASCII strings to inte­
 * gers for calculation. When we encounter a new digit, we multiply the
 * accumulated result by 10 and add the digit.
 * The stack contains integers-that is, we assume that Item is de­
 * fined to be int in Item.h, and that Item.h is also included in the stack
 * implementation (see, for example, Program 4.4).
 */
#include <stdio.h>
#include <string.h>
#include "ItemInt.h"
#include "STACK.h"

int main(int argc, char *argv[]) {
  char *a = argv[1];
  int i, N = strlen(a);
  STACKinit(N);

  for (i = 0; i < N; ++i) {
    if (a[i] == '+')
      STACKpush(STACKpop() + STACKpop());
    if (a[i] == '*')
      STACKpush(STACKpop() * STACKpop());
    if ((a[i] >= '0') && (a[i] <= '9'))
      STACKpush(0);
    while ((a[i] >= '0') && (a[i] <= '9'))
      STACKpush(10*STACKpop() + (a[i++]-'0'));
  }
  printf("%d \n", STACKpop());
  return 0;
}
