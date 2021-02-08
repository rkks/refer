/* Exercise 1.15: Give a sequence of input pairs that causes the weighted
 * quick-union algorithm with path compression by halving (Program 1.3) to
 * produce a path of length 4.
 */
#include <stdio.h>
#define N 16

int quickunion_weighted_with_halving(int p, int q, int *id, int *sz) {
	int i, j, cnt = 2;

	for (i = p; i != id[i]; i = id[i], cnt++) {
		id[i] = id[id[i]];
		cnt += 4;
	}
	for (j = q; j != id[j]; j = id[j], cnt++) {
		id[j] = id[id[j]];
		cnt += 4;
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
	cnt = quickunion_weighted_with_halving(p, q, id, sz);
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
	process_step(0, 1, id, sz);
	process_step(2, 3, id, sz);
	process_step(4, 5, id, sz);
	process_step(6, 7, id, sz);
	process_step(8, 9, id, sz);
	process_step(10, 11, id, sz);
	process_step(12, 13, id, sz);
	process_step(14, 15, id, sz);
	process_step(1, 2, id, sz);
	process_step(5, 6, id, sz);
	process_step(9, 10, id, sz);
	process_step(13, 14, id, sz);
	process_step(0, 4, id, sz);
	process_step(8, 12, id, sz);
	process_step(0, 8, id, sz);

	return 0;
}
