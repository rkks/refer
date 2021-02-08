/* Program 9.2: Array implementation of a priority queue
 * This implementation, which may be compared with the array
 * implementation for stack and queues that we considered in Chapter 4
 * (see Program 4.4), keeps the items in an unordered array. Items are
 * added to and removed from the end of the array, as in a stack.
 */
#include <stdlib.h>
#include <Item.h>

static Item *pq;
static int N;

void PQinit(int maxN) {
	pq = malloc(maxN*sizeof(Item));
	N = 0;
}

int PQempty() {
	return N == 0;
}

void PQinsert(Item v) {
	pq[N++] = v;
}

Item PQdelmax() {
	int j, max = 0;

	for (j = 1; j < N; ++j)
		if (less(pq[max], pq[j]))
			max = j;
	exch(pq[max], pq[N-1]);

	return pq[--N];
}
