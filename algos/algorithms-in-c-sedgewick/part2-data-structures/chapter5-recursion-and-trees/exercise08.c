/* Exercise 5.8: Give the figure corresponding to Figure 5.3 for the
 * result of recursive prefix-expression evaluation for the input
 * + * * 12 12 12 144.
 */
#include <stdio.h>
char *a;
int i;
int depth;

void print_indent() {
  int j;

  for (j = 0; j < depth; ++j)
    printf("  ");
}

int eval() {
  int x = 0, tmp1, tmp2, tmp3;

  while (a[i] == ' ')
    ++i;
  if (a[i] == '+') {
    print_indent();
    printf("eval() %s\n", a+(i++));
    ++depth;

    tmp1 = eval();
    tmp2 = eval();
    tmp3 = tmp1 + tmp2;
    print_indent();
    printf("return %d = %d+%d\n", tmp3, tmp1, tmp2);
    --depth;
    return tmp3;
  }
  if (a[i] == '*') {
    print_indent();
    printf("eval() %s\n", a+(i++));
    ++depth;

    tmp1 = eval();
    tmp2 = eval();
    tmp3 = tmp1 * tmp2;
    print_indent();
    printf("return %d = %d*%d\n", tmp3, tmp1, tmp2);
    --depth;
    return tmp3;
  }
  while ((a[i] >= '0') && (a[i] <= '9'))
    x = 10*x + (a[i++]-'0');

  print_indent();
  printf("eval() %d\n", x);
  return x;
}

int main(const int argc, char *argv[]) {
  a = argv[1];
  i = 0;
  depth = 0;

  eval();
  return 0;
}
