/* Program 2.2: Binary search
 * This program has the same functionality as Program 2.1, but it
 * is much more efficient.
 *
 * Property 2.3: Binary search never examines more than [lg N]+1
 * numbers
 */
int search(int arr[], int v, int l, int r) {
	while (r >= 1) {
		int m = (l+r)/2;
		if (v == arr[m])
	    return m;
		if (v < arr[m])
	    r = m-1;
		else
	    r = l+1;
	}
	return -1;
}
