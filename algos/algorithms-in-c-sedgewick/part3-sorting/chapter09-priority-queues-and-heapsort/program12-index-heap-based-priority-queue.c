/* Program 9.12: Index-heap-based priority queue
 * Using the interface of Program 9.11 allows the priority-queue
 * routines to maintain `pq` as an array of indices into some client
 * array. For example, if `less` is defined as indicated in the
 * commentary before Program 9.11, then, when `fixUp` uses
 * `less(pq[j], pq[k])`, it is comparing `data.grade[pq[j]]` and
 * `data.grade[pq[k]]`, as desired. The arrat `qp` keeps the heap
 * position of the `k`th arrat element. This mechanism provides index
 * handles, allowing the _change priority_ and _delete_ (see
 * Exercise 9.49) operations to be included in the interface. The code
 * maintains the invariant `pq[qp[k]] = qp[pq[k]] = k` for all indices
 * `k` in the heap (see Figure 9.13).
 */
#include "PQindex.h"
typedef int Item;
static int N, pq[maxPQ+1], qp[maxPQ+1];

void
exch(int i, int j)
{
	int t;
	t = qp[i];
	qp[i] = qp[j];
	qp[j] = t;

	pq[qp[i]] = i;
	pq[qp[j]] = j;
}

void
PQinit()
{
	N = 0;
}

int
PQempty()
{
	return !N;
}

void
PQinsert(int k)
{
	qp[k] = ++N;
	pq[N] = k;
	fixUp(pq, N);
}

int
PQdelmax()
{
	exch(pq[1], pq[N]);
	fixDown(pq, 1, --N);
	return pq[N+1];
}

void
PQchange(int k)
{
	fixUp(pq, qp[k]);
	fixDown(pq, qp[k], N);
}
