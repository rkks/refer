/* Program 5.1: Factorial function (recursive implementation)
 * This recursive function computes the function N!, using the standard
 * recursive definition. It returns the correct value when called with N
 * nonnegative and sufficiently small that N! can be represented as an `int`.
 */
int factorial(int N) {
  if (N == 0)
    return 1;
  return N*factorial(N-1);
}
