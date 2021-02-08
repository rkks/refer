/* Program 3.5: Sieve of Eratosthenes
 * The goal of this program is to set `a[i]` to 1 if `i` is prime,
 * and to 0 otherwise. First, it sets to 1 all array elements, to
 * indicate that no numbers are known to be nonprime. Then it sets
 * to 0 array elements corresponding to indices that are known to
 * be nonprime (multiples of known primes). If `a[i]` is still 1
 * after all multiples of smaller primes have been set to 0, then
 * we know it to be prime.
 *
 * Because the program uses an array consisting of the simplest
 * type of elements, 0-1 values, it would be more space efficient
 * if we explicitly used an array of bits, rather than one of
 * integers. Also some programming environments might require the
 * array to be global if N is huge, or we could allocate it
 * dynamically (see Program 3.6).
 */
#include <stdio.h>
#define N 10000

int main() {
  int i, j, a[N];

  for (i = 2; i < N; ++i)
    a[i] = 1;
  for (i = 2; i < N; ++i)
    if (a[i])
      for (j = i; i*j < N; ++j)
	a[i*j] = 0;
  for (i = 2; i < N; ++i)
    if (a[i])
      printf("%4d ", i);
  printf("\n");
}
