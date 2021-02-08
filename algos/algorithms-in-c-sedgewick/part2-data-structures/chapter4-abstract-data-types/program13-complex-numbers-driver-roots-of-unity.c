/* Program 4.13: Complex numbers driver (roots of unity)
 * This client program performs a computation on complex numbers using an ADT
 * that allows it to compute directly with the abstraction of interest by
 * declaring variables of type `Complex` and using them as arguments and return
 * values of functions. This program checks the ADT implemetation by computing
 * the powers of the roots of unity. It prints the table shown in Figure 4.12.
 *
 * Definition 4.4: A **first-class data type** is one for which we can have
 * potentially many different instances, and which we can assign to variables
 * which we can declare to hold the instances.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "COMPLEX.h"
#define PI 3.141592625

int main(int argc, char *argv[]) {
  int i, j, N = atoi(argv[1]);
  Complex t, x;

  printf("%dth complex roots of unity\n", N);
  for (i = 0; i < N; ++i) {
    float r = 2.0*PI*i/N;
    t = COMPLEXinit(cos(r), sin(r));
    printf("%2d %6.3f %6.3f ", i, Re(t), Im(t));
    for (x = t, j = 0; j < N-1; ++j)
      x = COMPLEXmult(t, x);
    printf("%6.3f %6.3f\n", Re(x), Im(x));
  }
  return 0;
}
