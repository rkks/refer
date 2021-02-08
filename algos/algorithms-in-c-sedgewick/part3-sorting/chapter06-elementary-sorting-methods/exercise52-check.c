/* Exercise 6.52: Add a function check to the array data type in
 * Programs 6.8 and 6.7, which tests whether or not the array is in
 * sorted order.
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
	check(a, l, r);
}

void check(Item a[], int l, int r) {
	int i;
	for (i = l; i < r; ++i)
		if (a[i] > a[i+1]) {
			printf("The array is not in sorted order.\n");
			return;
		}
	printf("The array is in sorted order.\n");
}
