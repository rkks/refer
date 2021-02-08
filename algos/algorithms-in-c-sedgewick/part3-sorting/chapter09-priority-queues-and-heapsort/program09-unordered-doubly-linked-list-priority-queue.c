/* Program 9.9: Unordered doubly-linked-list priority queue
 * This implementation of the _initialize_, _test if empty_, _insert_
 * and _delete the maximum_ routines from the interface of Program 9.8
 * uses only elementary operations to maintain an unordered list,
 * with head and tail nodes. We specify the structure `PQnode` to be
 * a doubly-linked list node (with a key and two links), and the
 * structure `pq` to be the list's head and tail links.
 */
#include <stdlib.h>
#include "Item.h"
#include "PQfull.h"

struct PQnode {
	Item key;
	PQlink prev, next;
};

struct pq {
	PQlink head, tail;
};

PQ
PQinit()
{
	PQ pq = malloc(sizeof *pq);
	PQlink h = malloc(sizeof *h),
	       t = malloc(sizeof *t);

	h->prev = t;
	h->next = t;
	t->prev = h;
	t->next = h;
	pq->head = h;
	pq->tail = t;

	return pq;
}

int
PQempty(PQ pq)
{
	return pq->head->next->next == pq->head;
}

PQlink
PQinsert(PQ pq, Item v)
{
	PQlink t = malloc(sizeof *t);
	t->key = v;
	t->next = pq->head->next;
	t->next = t->next->prev = t;
	t->prev = pq->head;
	pq->head->next = t;

	return t;
}

Item
PQdelmax(PQ pq)
{
	Item max;
	struct PQnode *t,
		      *x = pq->head->next;

	for (t = x; t->next != pq->head; t = t->next)
		if (t->key > x->key)
			x = t;
	max = x->key;
	x->next->prev = x->prev;
	x->prev->next = x->next;

	free(x);
	return max;
}
