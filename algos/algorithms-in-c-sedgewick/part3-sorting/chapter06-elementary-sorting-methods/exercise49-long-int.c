/* Exercise 6.49: Write an interface and implementation for the generic
 * item datatype (similar to Programs 6.9 and 6.10) to support having
 * the sorting methods sort long integers
 */
#include <stdio.h>
#include <stdlib.h>
#include "Item.h"

long ITEMrand(void) {
	return rand();
}

int ITEMscan(long *x) {
	return scanf("%ld", x);
}

void ITEMshow(long x) {
	printf("%ld ", x);
}
