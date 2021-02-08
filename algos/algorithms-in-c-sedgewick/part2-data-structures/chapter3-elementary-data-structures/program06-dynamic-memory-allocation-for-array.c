/* Program 3.6: Dynamic memory allocation for an array
 * To change the value of the maximum prime computed in Program
 * 3.5, we need to recompile the program. Instead, we can take the maximum desired number from the command line, and use it to
 * allocate space for the array at execution time, using the
 * library function `malloc` from `stdlib.c`. For example, if we
 * compile this program and use 1000000 as a command-line
 * argument, then we get all the primes less than 1 million (as
 * long as our computer is big and fast enough to make the
 * computation feasible); we can also debug with 100 (without
 * using much time or space). We will use this idiom frequently,
 * though, for brevity, we will omit the insufficient-memory test.
 */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  long i, j, N = atoi(argv[1]);
  int *a = malloc(N*sizeof(int));
  if (a == NULL) {
    printf("Insufficient memory.\n");
    return 1;
  }

  for (i = 2; i < N; ++i)
    a[i] = 1;
  for (i = 2; i < N; ++i)
    if (a[i])
      for (j = i; i*j < N; ++j)
	a[i*j] = 0;
  for (i = 2; i < N; ++i)
    if (a[i])
      printf("%ld ", i);
  printf("\n");

  free(a);
  return 0;
}
