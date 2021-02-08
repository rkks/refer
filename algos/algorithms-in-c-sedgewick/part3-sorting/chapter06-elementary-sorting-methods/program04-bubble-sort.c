/* Program 6.4: Bubble sort
 * For each i from l to r-1, the inner (j) loop puts the minimum element
 * among the elements in a[i], ..., a[r] into a[i] by passing from right
 * to left through the elements, compare-exchanging successive elements.
 * The smallest one moves on all such comparisons, so it "bubbles" to
 * the beginning. As in selection sort, as the index i travels from left
 * to right through the file, the elements to its left are in their
 * final position in the array.
 *
 * Property 6.3: Bubble sort uses about N^2/2 comparisons and
 *   N^2/2 exchanges on the average and in the worst case.
 *
 * Property 6.4: Insertion sort and bubble sort use a linear number of
 *   comparisons and exchanges for files with at most a constant number
 *   of inversions corresponding to each element.
 */
#include <stdio.h>
#include <stdlib.h>
typedef int Item;
#define key(A) (A)
#define less(A, B) (key(A) < key(B))
#define exch(A, B) { Item t = A; A = B; B = t; }
#define compexch(A, B) if (less(B, A)) exch(A, B)

void sort(Item a[], int l, int r) {
	int i, j;

	for (i = l; i < r; ++i)
		for (j = r; j > i; --j)
			compexch(a[j-1], a[j]);
}
