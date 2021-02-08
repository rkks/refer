#include <stdlib.h>
#include "Item.h"

int less(Item a, Item b) {
	return a->num < b->num;
}

int compare(const void *i, const void *j) {
	return less(*(Item *)i, *(Item *)j);
}

void sort(Item a[], int l, int r) {
	qsort(a, r-l+1, sizeof(Item), compare);
}
