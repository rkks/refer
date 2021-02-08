/* Exercise 3.63: Write a version of Program 3.16 that handles
 * three-dimensional arrays.
 */
#include <stdio.h>
#include <stdlib.h>

int ***malloc3d(int n, int m, int l) {
  int i, j, ***t = malloc(n*sizeof(int **));

  for (i = 0; i < n; ++i) {
    t[i] = malloc(m*sizeof(int *));
    for (j = 0; j < m; ++j)
      t[i][j] = malloc(l*sizeof(int));
  }
  return t;
}

void free3d(int ***arr3d, int n, int m) {
  int i, j;

  for (i = 0; i < n; ++i) {
    for (j = 0; j < m; ++j)
      free(arr3d[i][j]);
    free(arr3d[i]);
  }
  free(arr3d);
}
