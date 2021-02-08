/* Program 4.12: Stack with index items and no duplicates
 * This pushdown-stack implementation assumes that all items are integers
 * between `0` and `maxN-1`, so that it can maintain an array `t` that has a
 * nonzero value corresponding to each item in the stack. The array enables
 * `STACKpush` to test quickly whether its argument is already on the stack,
 * and to take no action if the teset succeeds. We use only one bit per entry
 * in `t`, so we could save space by using characters or bits instead of
 * integers, if desired (see Exercise 12.12).
 */
#include <stdlib.h>
static int *s, *t, N;

void STACKinit(int maxN) {
  int i;
  s = malloc(maxN*sizeof(int));
  t = malloc(maxN*sizeof(int));
  for (i = 0; i < maxN; ++i)
    t[i] = 0;
  N = 0;
}

int STACKempty() {
  return !N;
}

void STACKpush(int item) {
  if (t[item] == 1)
    return;
  s[N++] = item;
  t[item] = 1;
}

int STACKpop() {
  N--;
  t[s[N]] = 0;
  return s[N];
}
