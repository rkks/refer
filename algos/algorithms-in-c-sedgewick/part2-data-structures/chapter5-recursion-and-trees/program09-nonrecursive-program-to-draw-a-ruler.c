/* Program 5.9: Nonrecursive program to draw a ruler
 * In contrast to Program 5.8, we can also draw a ruler by first
 * drawing all the marks of length 1, then drawing all the marks of
 * length 2, and so forth. The variable `t` carries the length of the
 * marks and the variable `j` carries the number of marks in between two
 * successive marks of length `t`. The outer `for` loop increments `t`
 * and preserves the property j = 2^(t-1). The inner `for` loop draws
 * all the marks of length `t`.
 */
void rule(int l, int r, int h) {
  int i, j, t;

  for (t = 1, j = 1; t <= h; j += j, ++t)
    for (i = 0; l+j+i <= r; i += j+j)
      mark(l+j+i, t);
}
