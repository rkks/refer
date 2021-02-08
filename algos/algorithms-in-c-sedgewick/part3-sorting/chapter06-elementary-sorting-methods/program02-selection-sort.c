/* Program 6.2: Selection sort
 * For each i from l to r-1, exchange a[i] with the minimum element in
 * a[i], ..., a[r]. As the index i travels from left to right, the
 * elements to its left are in their final position in the array (and
 * will not be touched again), so the array is fully sorted when i
 * reaches the right end.
 *
 * Running time: O(N^2)
 * Disadvantage: its running time depends only slightly on the amount
 *   of order already in the file.
 *   i.e. it does not try to keep the existing order.
 * Powerful when: Sorting files with huge items and small keys.
 *
 * Property 6.1: Selection sort uses about N^2/2 comparisons and
 *   N exchanges.
 *
 * Property 6.6: Selection sort runs in linear time for files with large
 *   items and small keys.
 */
typedef int Item;
#define key(A) (A)
#define less(A, B) (key(A) < key(B))
#define exch(A, B) { Item t = A; A = B; B = t; }
#define compexch(A, B) if (less(B, A)) exch(A, B)

void sort(Item a[], int l, int r) {
	int i, j;

	for (i = l; i < r; ++i) {
		int min = i;
		for (j = i+1; j <= r; ++j)
			if (less(a[j], a[min]))
				min = j;
		exch(a[i], a[min]);
	}
}
