/* Program 4.21: Polynomial client (binomial coefficients)
 * This client program uses the polynomial ADT that is defined in the interface
 * Program 4.22 to perform algebraic manipulations with polynomials. It takes
 * an integer N and a floating-pointer number p from the command line, computes
 * (x+1)^N, and checks the result by evaluating the resulting polynomial at
 * x = p.
 */
#include <stdio.h>
#include <stdlib.h>
#include "POLY.h"

int main(int argc, char *argv[]) {
  int N = atoi(argv[1]);
  float p = atof(argv[2]);
  Poly t, x;
  int i;

  printf("Binomial coefficients\n");
  t = POLYadd(POLYterm(1, 1), POLYterm(1, 0));
  showPOLY(t);
  for (i = 0, x = t; i < N-1; ++i) {
    x = POLYmult(t, x);
    showPOLY(x);
  }
  printf("%f\n", POLYeval(x, p));
  return 0;
}
