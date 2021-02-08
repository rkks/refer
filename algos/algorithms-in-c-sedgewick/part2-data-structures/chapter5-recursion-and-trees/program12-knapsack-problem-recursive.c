/* Program 5.12: Knapsack problem (recursive implementation)
 * As we warned about the recursive solution to the problem of
 * computing the Fibonacci numbers, DO NOT USE THIS PROGRAM, because it
 * will take exponential time and therefore may not ever run to
 * completion even for small problems. It does, however, represent a
 * compact solution that we can improve easily (see Program 5.13). This
 * code assumes that items are structures with a size and a value,
 * defined with
 *
 * typedef struct { int size; int val; } Item;
 *
 * and that we have an array of N items of type Item. For each possible
 * item, we calculate (recursively) the maximum value that we could
 * achieve by including that item, then take the maximum of all those
 * values.
 */
int knap(int cap) {
  int i, space, max, t;
  for (i = 0, max = 0; i < N; ++i)
    if ((space = cap-items[i].size) >= 0)
      if ((t = knap(space) + items[i].val) > max)
        max = t;
  return max;
}
