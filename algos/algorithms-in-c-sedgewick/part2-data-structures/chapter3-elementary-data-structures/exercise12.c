/* Exercise 3.12: Modify our implementation of the sieve of Eratosthenes
 * (Program 3.5) to use an array of (i) chars; and (ii) bits. Determine the
 * effects of these changes on the amount of space and time used by the
 * program.
 */
#include <stdio.h>
#include <stdbool.h>
#define N 10000

void sieve_char(char *arr) {
  int i, j;

  for (i = 2; i < N; ++i)
    arr[i] = 1;
  for (i = 2; i < N; ++i)
    if (arr[i])
      for (j = i; i*j < N; ++j)
        arr[i*j] = 0;
}

void sieve_bool(bool *arr) {
  int i, j;

  for (i = 2; i < N; ++i)
    arr[i] = 1;
  for (i = 2; i < N; ++i)
    if (arr[i])
      for (j = i; i*j < N; ++j)
        arr[i*j] = 0;
}

int main() {
  int i;
  char a[N];
  bool b[N];

  sieve_char(a);
  sieve_bool(b);
  for (i = 2; i < N; ++i)
    if (a[i] != b[i]) {
      printf("sieve_bool and sieve_char disagrees.\n");
      return 1;
    }
  printf("sieve_bool and sieve_char agrees.\n");
  return 0;
}
