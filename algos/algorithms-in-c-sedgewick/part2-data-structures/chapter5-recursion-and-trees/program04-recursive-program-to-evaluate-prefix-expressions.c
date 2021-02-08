/* Program 5.4: Recursive program to evaluate prefix expressions
 * To evaluate a prefix expression, we either convert a number from ASCII to
 * binary (in the `while` loop at the end), or perform the operation indicated
 * by the first character in the expression on the two operands, evaluated
 * recursively. This function is recursive, but it uses a global array
 * containing the expression and an index to the current character in the
 * expression. The pointer is advanced past each subexpression evaluated.
 */
#include <stdio.h>
char *a;
int i;

int eval() {
  int x = 0;

  while (a[i] == ' ')
    ++i;
  if (a[i] == '+') {
    ++i;
    return eval() + eval();
  }
  if (a[i] == '*') {
    ++i;
    return eval() * eval();
  }
  while ((a[i] >= '0') && (a[i] <= '9'))
    x = 10*x + (a[i++]-'0');
  return x;
}

int main(int argc, char *argv[]) {
  int x;
  a = argv[1];
  i = 0;
  printf("input: %s\n", a);
  x = eval();
  printf("result: %d\n", x);
  return 0;
}
