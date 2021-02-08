/* Program 6.10: Sample implementation for item data type
 * This code implements the three functions ITEMrand, ITEMscan, and
 * ITEMshow that are declared in Program 6.9. In this code, we refer to
 * the type of the data directly with double, use explicit
 * floating-point options in scanf and printf, and so forth. We include
 * the interface file Item.h so that we will discover at compile time
 * any discrepancies between interface and implementation.
 */
#include <stdio.h>
#include <stdlib.h>
#include "Item.h"

double ITEMrand(void) {
	return 1.0*rand()/RAND_MAX;
}

int ITEMscan(double *x) {
	return scanf("%f", x);
}

void ITEMshow(double x) {
	printf("%7.5f ", x);
}
