/* Exercise 3.13: Use the sieve of Eratosthenes to determine the number of
 * primes less than N, for N = 10^3, 10^4, 10^5, and 10^6.
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

void sieve_bool(long, bool *);
void sieve_char(long, char *);

int main() {
  long i, n = 1000000, cnt = 0;
  bool *arr = (bool *) malloc(n*sizeof(bool));
  if (arr == NULL) {
    printf("MEMORY ERROR\n");
    return 1;
  }

  sieve_bool(n, arr);

  for (i = 0; i < 1000; ++i)
    if (arr[i])
      ++cnt;
  printf("#primes <= 1000: %ld\n", cnt);

  for (; i < 10000; ++i)
    if (arr[i])
      ++cnt;
  printf("#primes <= 10000: %ld\n", cnt);

  for (; i < 100000; ++i)
    if (arr[i])
      ++cnt;
  printf("#primes <= 100000: %ld\n", cnt);

  for (; i < n; ++i)
    if (arr[i])
      ++cnt;
  printf("#primes <= 1000000: %ld\n", cnt);
  free(arr);
  return 0;
}
