/* Exercise 4.16: Combine Program 4.2 and Program 4.3 into a
 * single module that uses two different stack ADTs: A stack of
 * integers and a stack of operators.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ex16-ItemInt.h"
#include "ex16-ItemChar.h"

#include "ex16-STACKINT.h"
#include "ex16-STACKCHAR.h"

int main(int argc, char *argv[]) {
  char *a = argv[1];
  int i, j = 0, N = strlen(a);
  char *postfix = malloc(sizeof(char)*N);
 
  // infix to postfix
  STACKCHARinit(N);

  for (i = 0; i < N; ++i) {
    if (a[i] == ')') {
      postfix[j++] = STACKCHARpop();
      postfix[j++] = ' ';
    }
    if ((a[i] == '+') || (a[i] == '*'))
      STACKCHARpush(a[i]);
    if ((a[i] >= '0') && (a[i] <= '9')) {
      postfix[j++] = a[i];
      postfix[j++] = ' ';
    }
  }
  postfix[j] = '\0';

  // compute postfix
  STACKINTinit(j);

  for (i = 0; i < j; ++i) {
    if (postfix[i] == '+')
      STACKINTpush(STACKINTpop() + STACKINTpop());
    if (postfix[i] == '*')
      STACKINTpush(STACKINTpop() * STACKINTpop());
    if ((postfix[i] >= '0') && (postfix[i] <= '9'))
      STACKINTpush(0);
    while ((postfix[i] >= '0') && (postfix[i] <= '9'))
      STACKINTpush(10*STACKINTpop() + (postfix[i++]-'0'));
  }
  return STACKINTpop();
}
