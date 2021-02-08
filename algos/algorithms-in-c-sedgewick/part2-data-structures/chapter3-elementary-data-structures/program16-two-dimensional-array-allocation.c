/* Program 3.16: Two-dimensional array allocation
 * This function dynamically allocates the memory for a two-dimensional array,
 * as an array of arrays. We first allocate an array of pointers, then allocate
 * memory for each row. With this function, the statement
 *
 * int **a = malloc2d(M, N);
 *
 * allocates an M-by-N array of integers.
 */
#include <stdio.h>
#include <stdlib.h>

int **malloc2d(int r, int c) {
  int i, **t = malloc(r*sizeof(int *));

  for (i = 0; i < r; ++i)
    t[i] = malloc(c*sizeof(int));
  return t;
}

void free2d(int **arr2d, int n) {
  int i;

  for (i = 0; i < n; ++i)
    free(arr2d[i]);
  free(arr2d);
}

int main() {
  int i, j, m = 10, n = 7,
      **arr = malloc2d(m, n);

  for (i = 0; i < m; ++i) {
    int *row = arr[i];
    for (j = 0; j < n; ++j)
      row[j] = i*j;
  }
   
  for (i = 0; i < m; ++i) {
    for (j = 0; j < n; ++j)
      printf("%d\t", arr[i][j]);
    printf("\n");
  }

  free2d(arr, m);
  return 0;
}
