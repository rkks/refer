/* Program 1.4: Path compression by halving
 * This program halves the length of any path that we traverse. The net result
 * of this change is that the trees become almost completely flat after a long
 * sequence of operations.
 */
#include <stdio.h>
#define N 10000

int main() {
	int i, j, p, q, id[N], sz[N];

	for (i = 0; i < N; ++i) {
		id[i] = i;
		sz[i] = 1;
	}
	while (scanf("%d %d\n", &p, &q) == 2) {
		for (i = p; i != id[i]; i = id[i])
	    id[i] = id[id[i]];
		for (j = q; j != id[j]; j = id[j])
	    id[j] = id[id[j]];
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
