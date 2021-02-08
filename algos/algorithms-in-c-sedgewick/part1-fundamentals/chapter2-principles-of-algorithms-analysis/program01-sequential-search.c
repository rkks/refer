/* Program 2.1: Sequential search
 * This function checks whether the number v is among a previously
 * stored set of numbers in a[l], a[l+1], ..., a[r], by comparing
 * against each number sequentially, starting at the beginning. If
 * we reach the end without finding the number sought, then we
 * return the value -1. Otherwise, we return the index of the
 * array position containing the number.
 *
 * Property 2.1: Sequential search examines N numbers for each
 * unsuccessful search and about N/2 numbers for each successful
 * search on the average.
 *
 * Property 2.2: Sequential search in an ordered table examines N
 * numbers for each search in the worst case and about N/2 numbers
 * for each search on the average.
 */
int search(int arr[], int v, int l, int r) {
	int i;

	for (i = l; i <= r; ++i)
	if (v == arr[i])
		return i;
	return -1;
}
