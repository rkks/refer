/* Exercise 1.16: Show how to modify Program 1.3 to implement full path
 * compression, where we complete each union operation by making every node
 * that we touch point to the root of the new tree.
 */
#include <stdio.h>
#define N 10000

int main() {
	int i, i2, j, j2, p, q, id[N], sz[N];

	for (i = 0; i < N; i++) {
		id[i] = i;
		sz[i] = 1;
	}
	while (scanf("%d %d\n", &p, &q) == 2) {
		for (i = p; i != id[i]; i = id[i])
			;
		for (i2 = p; i2 != id[i2]; i2 = id[i2])
			id[i2] = i;
		for (j = q; j != id[j]; j = id[j])
			;
		for (j2 = q; j2 != id[j2]; j2 = id[j2])
			id[j2] = j;
		if (i == j)
			continue;
		if (sz[i] < sz[j]) {
			id[i] = j;
			sz[j] += sz[i];
		} else {
			id[j] = i;
			sz[i] += sz[j];
		}
		printf(" %d %d\n", p, q);
	}
	return 0;
}
