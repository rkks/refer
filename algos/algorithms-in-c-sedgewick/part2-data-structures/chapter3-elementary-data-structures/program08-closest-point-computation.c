/* Program 3.8: Closest-point computation
 * This program illustrates the use of an array of structures, and
 * is representative of the typical situation where we save items
 * in an array to process them later, during some computation. It
 * counts the number of pairs of N randomly generated points in
 * the unit square that can be connected by a straight line of
 * length than d, using the data type for points described in
 * Section 3.1. The running time is O(N^2), so this program cannot
 * be used for huge N. Program 3.20 provides a faster solution.
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "Point.h"

float randFloat() {
  return 1.0*rand()/RAND_MAX;
}

int main(int argc, char *argv[]) {
  float d = atof(argv[2]);
  int i, j, cnt = 0, N = atoi(argv[1]);
  point *a = malloc(N*sizeof(*a));

  for (i = 0; i < N; ++i) {
    a[i].x = randFloat();
    a[i].y = randFloat();
  }
  for (i = 0; i < N; ++i)
    for (j = i+1; j < N; ++j)
      if (distance(a[i], a[j]) < d)
	++cnt;
  printf("%d edges shorter than %f\n", cnt, d);
  return 0;
}
