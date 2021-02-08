/* Exercise 1.5: Do Exercise 1.4, but use the quick-union algorithm (Program
 * 1.2).
 */
#include <stdio.h>
#define N 7

int quickunion(int p, int q, int *id) {
	int i, j, cnt = 2;
	for (i = p; i != id[i]; i = id[i], cnt++)
		cnt++;
	for (j = q; j != id[j]; j = id[j], cnt++)
		cnt++;
	if (i == j)
		return cnt;
	id[i] = j;
	return cnt+1;
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

void process_step(const int p, const int q, int *id) {
	int cnt;
	cnt = quickunion(p, q, id);
	printf("%d %d\t", p, q);
	print_id(id);
	printf("with %d accesses\n", cnt);
}

int main() {
	int i, id[N];

	for (i = 0; i < N; i++)
	id[i] = i;
	print_header(id);
	process_step(0, 2, id);
	process_step(1, 4, id);
	process_step(2, 5, id);
	process_step(3, 6, id);
	process_step(0, 4, id);
	process_step(6, 0, id);
	process_step(1, 3, id);

	return 0;
}
