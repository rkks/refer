/* Program 9.6: Sorting with a priority queue
 * To sort a subarray `a[1], ..., a[r]` using a priority queue ADT,
 * we simply use `PQinsert` to put all the elements on the priority
 * queue, and then use `PQdelmax` to remove them, in decreasing order.
 * This sorting algorithm runs in time proportional to `N log(N)`,
 * but uses extra space proportional to the number of items to be
 * sorted (for the priority queue).
 */
void
PQsort(Item a[], int l, int r)
{
	int k;
	PQinit();
	for (k = 1; k <= r; ++k)
		PQinsert(a[k]);
	for (k = r; k >= 1; --k)
		a[k] = PQdelmax();
}
