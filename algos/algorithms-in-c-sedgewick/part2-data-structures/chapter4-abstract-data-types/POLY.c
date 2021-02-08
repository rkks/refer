/* Program 4.23: Array implementation of polynomial ADT
 * In this implementation of a first-class ADT for polynomials, a polynomial is
 * a structure containing the degree and a pointer to an array of coefficients.
 * For simplicity in this code, each addition operation modifies one of its
 * arguments and each multiplication operation creates a new objects. Another
 * ADT operation to destroy objects (and to free the associated memory) might
 * be needed for some applications.
 */
#include <stdio.h>
#include <stdlib.h>
#include "POLY.h"
struct poly {
  int N;
  int *a;
};

void showPOLY(Poly poly) {
  int i;

  printf("%d ", poly->a[0]);
  for (i = 1; i < poly->N; ++i) {
    printf("+ %dx^%d ", poly->a[i], i);
  }
  printf("\n");
}

Poly POLYterm(int coeff, int exp) {
  int i;
  Poly t = malloc(sizeof *t);
  t->a = malloc((exp+1)*sizeof(int));
  t->N = exp+1;
  t->a[exp] = coeff;
  for (i = 0; i < exp; ++i)
    t->a[i] = 0;
  return t;
}

Poly POLYadd(Poly p, Poly q) {
  int i;
  Poly t;

  if (p->N < q->N) {
    t = p;
    p = q;
    q = t;
  }
  for (i = 0; i < q->N; ++i)
    p->a[i] += q->a[i];
  return p;
}

Poly POLYmult(Poly p, Poly q) {
  int i, j;
  Poly t = POLYterm(0, (p->N-1)+(q->N-1));

  for (i = 0; i < p->N; ++i)
    for (j = 0; j < q->N; ++j)
      t->a[i+j] += p->a[i]*q->a[j];
  return t;
}

float POLYeval(Poly p, float x) {
  int i;
  double t = 0.0;

  for (i = p->N-1; i >= 0; --i)
    t = t*x + p->a[i];
  return t;
}
