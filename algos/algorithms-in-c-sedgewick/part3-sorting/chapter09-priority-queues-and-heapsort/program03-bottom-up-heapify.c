/* Definition 9.2:
 * A tree is _heap-ordered_ if the key in each node is larger than
 * or equal to the keys in all of that node's children (if any).
 * Equivalently, the key in each node of a heap-ordered tree is
 * similar than or equal to the key in that node's parent (if any).
 */

/* Property 9.1:
 * No node in a heap-ordered tree has a key larger than the key at
 * the root.
 */

/* Definition 9.3:
 * A _heap_ is a set of nodes with keys arranged in a complete
 * heap-ordered binary tree, represented as an array.
 */

/* Program 9.3: Bottom-up heapify
 * To restore the heap condition when a node's priority is increased,
 * we move up the heap, exchanging the node at position `k` with its
 * parent (at position `k/2`) if necessary, continuing as long as
 * `a[k/2] < a[k]` or until we reach the top of the heap.
 */
void
fixUp(Item a[], int k)
{
	while (k > 1 && less(a[k/2], a[k]))
	{
		exch(a[k], a[k/2]);
		k = k/2;
	}
}
