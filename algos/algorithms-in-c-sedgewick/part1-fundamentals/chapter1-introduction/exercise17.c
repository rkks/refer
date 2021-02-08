/* Exercise 1.17: Do Exercise 1.4, but use the weighted quick-union algorithm
 * with full path compression (see Exercise 1.16).
 */
#include <stdio.h>
#define N 7

int quickunion_weighted_with_path_compression(int p, int q, int *id, int *sz) {
	int i, i2, j, j2, cnt = 4;

	for (i = p; i != id[i]; i = id[i], ++cnt)
		++cnt;
	for (i2 = p; i2 != id[i2]; i2 = id[i2], ++cnt) {
		id[i2] = i;
		++cnt;
	}
	for (j = q; j != id[j]; j = id[j], ++cnt)
		++cnt;
	for (j2 = q; j2 != id[j2]; j2 = id[j2], ++cnt) {
		id[j2] = j;
		++cnt;
	}
	if (i == j)
		return cnt;
	if (sz[i] < sz[j]) {
		id[i] = j;
		sz[j] += sz[i];
	} else {
		id[j] = i;
		sz[i] += sz[j];
	}
	return cnt+5;
}

void print_id(const int *id) {
	int i;
	
	for (i = 0; i < N; i++)
		printf("%d ", id[i]);
}

void print_header(const int *id) {
	printf("p q\t");
	print_id(id);
	printf("\n\n");
}

void process_step(const int p, const int q, int *id, int *sz) {
	int cnt;
	cnt = quickunion_weighted_with_path_compression(p, q, id, sz);
	printf("%d %d\t", p, q);
	print_id(id);
	printf("with %d accesses\n", cnt);
}

int main() {
	int i, id[N], sz[N];

	for (i = 0; i < N; i++) {
		id[i] = i;
		sz[i] = 1;
	}
	print_header(id);
	process_step(0, 2, id, sz);
	process_step(1, 4, id, sz);
	process_step(2, 5, id, sz);
	process_step(3, 6, id, sz);
	process_step(0, 4, id, sz);
	process_step(6, 0, id, sz);
	process_step(1, 3, id, sz);

	return 0;
}
