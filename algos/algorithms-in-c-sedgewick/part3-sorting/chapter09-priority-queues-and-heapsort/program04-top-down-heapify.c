/* Program 9.4: Top-down heapify
 * To restore the heap condition when a node's priority is decreased,
 * we move down the heap, exchanging the node at position `k` with
 * the larger of that node's two children if necessary and stopping
 * when the node at `k` is not smaller than either child or the bottom
 * is reached. Note that if `N` is even and `k` is `N/2`, then the
 * node at `k` has only one child -- this case must be treated
 * properly!
 *
 * The inner loop in this program has two distinct exits: one for the
 * case that the bottom of the heap is hit, and another for the case
 * that the heap condition is satisfied somewhere in the interior of
 * the heap. It is a prototypical example of the need for the `break`
 * construct.
 */
void
fixDown(Item a[], int k, int N)
{
	int j;
	while (2*k <= N)
	{
		j = 2*k;
		if (j < N && less(a[j], a[j+1]))
			++j;
		if (!less(a[k], a[j]))
			break;
		exch(a[k], a[j]);
		k = j;
	}
}
