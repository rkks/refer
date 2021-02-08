/* Exercise 6.3: Give an implementation of insertion sort with the inner
 * loop coded as a `while` loop that terminates on one of two
 * conditions, as described in the text.
 */
#include <stdio.h>
#include <stdlib.h>
typedef int Item;
#define key(A) (A)
#define less(A, B) (key(A) < key(B))
#define exch(A, B) { Item t = A; A = B; B = t; }
#define compexch(A, B) if (less(B, A)) exch(A, B)

void insertion(Item a[], int l, int r) {
	int i;

	for (i = r; i > l; --i)
		compexch(a[i-1], a[i]);
	for (i = l+2; i <= r; ++i) {
		int j = i;
		Item v = a[i];
		while (less(v, a[j-1]) || less(a[j], a[j-1])) {
			a[j] = a[j-1];
			--j;
		}
		a[j] = v;
	}
}

int main(const int argc, char *argv[]) {
	int i, N = atoi(argv[1]), sw = atoi(argv[2]);
	int *a = malloc(N*sizeof(int));

	if (sw)
		for (i = 0; i < N; ++i)
			a[i] = 1000*(1.0*rand()/RAND_MAX);
	else
		while (scanf("%d", &a[N]) == 1)
			++N;

	insertion(a, 0, N-1);

	for (i = 0; i < N; ++i)
		printf("%3d ", a[i]);
	printf("\n");
	free(a);
	return 0;
}
