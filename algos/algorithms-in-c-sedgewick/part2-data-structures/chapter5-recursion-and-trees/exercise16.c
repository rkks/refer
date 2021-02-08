/* Exercise 5.16: Write a recursive program that finds the maximum
 * element in an array, based on comparing the first element in the
 * array against the maximum element in the rest of the array (computed
 * recursively).
 */
#include <stdio.h>
#include <stdlib.h>
int max(const int *arr, const int size) {
  int first = *arr, rest;
  if (size == 1)
    return first;
  rest = max(arr+1, size-1);
  return (first > rest) ? first : rest;
}

int main(const int argc, char *argv[]) {
  int *arr = malloc((argc-1)*sizeof(int)),
      i;

  printf("input: ");
  for (i = 1; i < argc; ++i) {
    arr[i-1] = atoi(argv[i]);
    printf("%d ", arr[i-1]);
  }
  printf("\nmax: %d\n", max(arr, argc-1));
  free(arr);
  return 0;
}
