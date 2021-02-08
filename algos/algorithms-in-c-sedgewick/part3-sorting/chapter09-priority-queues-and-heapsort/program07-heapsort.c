/* Property 9.4: Bottom-up heap construction takes linear time.
 */

/* Property 9.5: Heapsort uses fewer than `2N log(N)` comarisons
 * to sort `N` elements.
 */

/* Program 9.7: Heapsort
 * Using `fixDown` directly gives the classical heapsort algorithm.
 * The `for` loop constructs the heap; then, the `while` loop exchanages
 * the largest elements with the final element in the array and repair
 * the heap, continuing until the heap is empty. The pointer `pq` to
 * a `a[l-1]` allows the code to treat the subarray passed to it as an
 * array with the first element at index `1`, for the array
 * representation of the complete tree (see Figure 9.2). Some
 * programming environmentss may disallow this usage.
 */
void
heapsort(Item a[], int l, int r)
{
	int k, N = r-l+1;
	Item *pq = a+l-1;

	for (k = N/2; k >= 1; --k)
		fixDown(pq, k, N);
	while (N > 1)
	{
		exch(pq[1], pq[N]);
		fixDown(pq, 1, --N);
	}
}
