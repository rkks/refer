/* Program 5.13: Knapsack problem (dynamic programming)
 * This mechanical modification to the code of Program 5.12 reduces the
 * running time from exponential to linear. We simply save any function
 * values that we compute, then retrieve any saved values whenever we
 * need them (using a sentinel value to represent unknown values),
 * rather than making recursive calls. We save the index of the item,
 * so that we can reconstruct the contents of the knapsack after the
 * computation, if we wish: `itemKnow[M]` is in the knapsack, the
 * remaining contents are the same as for the optimal knapsack of size
 * `M-itemKnow[M].size` so `itemKnown[M-items[M].size]` is in the
 * knapsack, and so forth.
 *
 * Property 5.3: Dynamic programming reduces the running time of a
 * recursive function to be at most the time required to evaluate the
 * function for all arguments less than or equal to the given argument,
 * treating the cost of a recursive call as constant.
 */
int knap(int M) {
  int i, space, max, maxi, t;
  if (maxKnown[M] != unknown)
    return maxKnown[M];
  for (i = 0, max = 0; i < N; ++i)
    if ((space = M-items[i].size) >= 0)
      if ((t = knap(space) + items[i].val) > max) {
        max = t;
        maxi = i;
      }
  maxKnown[M] = max;
  itemKnown[M] = items[maxi];
  return max;
}
