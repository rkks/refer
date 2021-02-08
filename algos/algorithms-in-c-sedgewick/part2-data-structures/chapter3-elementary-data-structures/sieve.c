#include <stdbool.h>

void sieve_int(long n, int *arr) {
  long i, j;

  for (i = 2; i < n; ++i)
    arr[i] = 1;
  for (i = 2; i < n; ++i)
    if (arr[i])
      for (j = i; i*j < n; ++j)
	arr[i*j] = 0;
}

void sieve_char(long n, char *arr) {
  long i, j;

  for (i = 2; i < n; ++i)
    arr[i] = 1;
  for (i = 2; i < n; ++i)
    if (arr[i])
      for (j = i; i*j < n; ++j)
	arr[i*j] = 0;
}  

void sieve_bool(long n, bool *arr) {
  long i, j;

  for (i = 2; i < n; ++i)
    arr[i] = 1;
  for (i = 2; i < n; ++i)
    if (arr[i])
      for (j = i; i*j < n; ++j)
	arr[i*j] = 0;
} 
