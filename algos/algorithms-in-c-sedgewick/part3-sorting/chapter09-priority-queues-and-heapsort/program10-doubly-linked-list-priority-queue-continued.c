/* Program 9.10 Doubly-linked-list priority queue (continued)
 * The overhead of maintaining doubly-linked lists is justified by the
 * fact that the _change priority_, _delete_, and _join_ operations
 * all are also implemented in constant time, again using only
 * elementary operations on the lists (see Chapter 3 for more details
 * on doubly linked lists).
 */
void
PQchange(PQ pq, PQlink x, Item v)
{
	x->key = v;
}

void
PQdelete(PQ pq, PQlink x)
{
	x->next->prev = x->prev;
	x->prev->next = x->next;
	free(x);
}

void
PQjoin(PQ a, PQ b)
{
	a->tail->prev->next = b->head->next;
	b->head->next->prev = a->tail->prev;
	a->head->prev = b->tail;
	b->tail->next = a->head;

	free(a->tail);
	free(b->head);
}
