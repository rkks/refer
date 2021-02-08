/* Property 9.2:
 * The __insert__ and __delete the maximum__ operations for the
 * priority queue abstract data type can be implemented with
 * heap-ordered trees such that __insert__ requires no more than
 * `lg(N)` comparisons and __delete the maximum__ no more than
 * `2lg(N)` comparisons, when performed on an `N`-item queue.
 */

/* Program 9.5: Heap-based priority queue
 * To implement `PQinsert`, we increment `N` by `1`, add the new
 * element at the end of the heap, then use `fixUp` to restore the
 * heap condition. For `PQdelmax`, the size of the heap has to
 * decrease by `1`, so we take the value to be returned from `pq[1]`
 * then reduce the size of the heap by moving `pq[N]` to `pq[1]` and
 * using `fixDown` to restore the heap condition. The implementation
 * of `PQinit` and `PQempty` are trivial. The first position in the
 * array, `pq[0]`, is not used, but may be available as a sentinel
 * for some implementations.
 */
#include <stdlib.h>
#include <Item.h>

static Item *pq;
static int N;

void
PQinit(int maxN)
{
	pq = malloc((maxN+1)*sizeof(Item));
	N = 0;
}

int
PQempty()
{
	return N == 0;
}

void
PQinsert(Item v)
{
	pq[++N] = v;
	fixUp(pq, N);
}

Item
PQdelmax()
{
	exch(pq[1], pq[N]);
	fixDown(pq, 1, N-1);
	return pq[N--];
}

/* Property 9.3:
 * The __change priority__, __delete__, and __replace the maximum__
 * operations for the priority queue abstract data type can be
 * implemented with heap-ordered trees such that no more than
 * `2log(N)` comparisons are required for any operation on an
 * `N`-item queue.
 */
