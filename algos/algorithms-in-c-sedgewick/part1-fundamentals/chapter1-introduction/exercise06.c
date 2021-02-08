/* Exercise 1.6: Give the contents the id array after each union operation for
 * the weighted quick-union algorithm running on the examples corresponding to
 * Figure 1.7 and Figure 1.8.
 */
#include <stdio.h>
#define N 10

void quickunion_weighted(int p, int q, int *id, int *sz) {
	int i, j;

	for (i = p; i != id[i]; i = id[i])
		;
	for (j = q; j != id[j]; j = id[j])
		;
	if (i == j)
		return;
	if (sz[i] < sz[j]) {
		id[i] = j;
		sz[j] += sz[i];
	} else {
		id[j] = i;
		sz[i] += sz[j];
	}
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
	quickunion_weighted(p, q, id, sz);
	printf("%d %d\t", p, q);
	print_id(id);
	printf("\n");
}

int main() {
	int i, id[N], sz[N];

	printf("// Reproduce Figure 1.7\n");
	for (i = 0; i < N; i++) {
		id[i] = i;
		sz[i] = 1;
	}
	print_header(id);
	process_step(3, 4, id, sz);
	process_step(4, 9, id, sz);
	process_step(8, 0, id, sz);
	process_step(2, 3, id, sz);
	process_step(5, 6, id, sz);
	process_step(2, 9, id, sz);
	process_step(5, 9, id, sz);
	process_step(7, 3, id, sz);
	process_step(4, 8, id, sz);
	process_step(5, 6, id, sz);
	process_step(0, 2, id, sz);
	process_step(6, 1, id, sz);

	printf("\n// Reproduce Figure 1.8\n");
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
	process_step(1, 2, id, sz);
	process_step(5, 6, id, sz);
	process_step(0, 7, id, sz);
	process_step(8, 7, id, sz);
	return 0;
}
