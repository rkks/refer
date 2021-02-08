/* Program 5.10: Fibonacci numbers (recursive implementation)
 * This program, although compact and elegant, is not usable because it
 * takes exponential time to compute F_N. The running time to compute
 * F_{N+1} is \phi \approx 1.618 times (the golden ratio) as long as the
 * running time to compute F_N. For example, since \phi^9 > 60, if we
 * notice that our computer takes about a second to compute F_N, we know
 * that it will take more than a minute to compute F_{N+9} and more than
 * an hour to compute F_{N+18}.
 *
 * DO NOT USE THIS PROGRAM.
 */
int F(int i) {
  if (i < 1) return 0;
  if (i == 1) return 1;
  return F(i-1) + F(i-2);

  /* By contrast, it is easy to compute in linear time, by computing
   * the first N Fibonacci numbers and storing them in an array:
   *
   * F[0] = 0; F[1] = 1;
   * for (i = 2; i <= N; ++i)
   *   F[i] = F[i-1] + F[i-2]
   */
}
