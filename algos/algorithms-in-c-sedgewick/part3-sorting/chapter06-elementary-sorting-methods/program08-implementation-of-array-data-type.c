/* Program 6.8: Implementation of array data type
 * This code provides implementations of the functions defined in
 * Program 6.7, again using the item types and basic functions for
 * processing them that are defined in a separate interface
 * (see Program 6.9).
 */
#include <stdio.h>
#include <stdlib.h>
#include "Item.h"
#include "Array.h"

void randinit(Item a[], int N) {
	int i;
	for (i = 0; i < N; ++i)
		a[i] = ITEMrand();
}

void scaninit(Item a[], int *N) {
	int i;
	for (i = 0; i < *N; ++i)
		if (ITEMscan(&a[i]) == EOF)
			break;
	*N = i;
}

void show(Item a[], int l, int r) {
	int i;
	for (i = l; i <= r; ++i)
		ITEMshow(a[i]);
	printf("\n");
}
