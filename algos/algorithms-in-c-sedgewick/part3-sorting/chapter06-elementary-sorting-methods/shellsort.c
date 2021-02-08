#include "Item.h"

void sort(Item a[], int l, int r) {
	int i, j, h;

	for (h = 1; h <= (r-1)/9; h = 3*h+1) ;
	for ( ; h > 0; h /= 3)
		for (i = l+h; i <= r; ++i) {
			j = i;
			Item v = a[i];
			while (j >= l+h && less(v, a[j-h])) {
				a[j] = a[j-h];
				j -= h;
			}
			a[j] = v;
		}
}
