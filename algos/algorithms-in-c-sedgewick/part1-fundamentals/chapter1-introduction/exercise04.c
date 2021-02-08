/* Exercise 1.4: Show the contents of the id array after each union operation
 * when you use the quick-find algorithm (Program 1.1) to solve the
 * connectivity problem for the sequence 0-2, 1-4, 2-5, 3-6, 0-4, 6-0, and 1-3.
 * Also give the number of times the program accesses the id array for each
 * input pair.
 */
#include <stdio.h>
#define N 7

int quickfind(int p, int q, int *id) {
  int i, t, cnt = 2;

	if (id[p] == id[q])
		return cnt;
	cnt++;
	for (t = id[p], i = 0; i < N; i++) {
		cnt++;
		if (id[i] == t) {
			id[i] = id[q];
			cnt += 2;
		}
	}
	return cnt;
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
	cnt = quickfind(p, q, id);
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
