/* Program 5.2: A questionable recursive program
 * If the argument N is odd, this function calls itself with 3N + 1 as an
 * argument; if N is even, it calls itself with N/2 as an argument. We cannot
 * use induction to prove that this program terminates, because not every
 * recursive call uses an argument smaller than the one given.
 */
int puzzle(int N) {
  if (N == 1)
    return 1;
  if (N&1)
    return puzzle(3*N+1);
  else
    return puzzle(N>>1);
}
