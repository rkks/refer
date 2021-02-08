/* Exercise 4.26: Modify Program 4.8 to use path compression by halving.
 */
#include <stdio.h>
#include "UF.h"
#define N 10

int *UFid();

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
	UFunion(p, q);
	printf("%d %d\t", p, q);
	print_id(id);
	printf("\n");
}

int main() {
	UFinit(N);

	print_header(UFid());
	process_step(3, 4, UFid());
	process_step(4, 9, UFid());
	process_step(8, 0, UFid());
	process_step(2, 3, UFid());
	process_step(5, 6, UFid());
	process_step(2, 9, UFid());
	process_step(5, 9, UFid());
	process_step(7, 3, UFid());
	process_step(4, 8, UFid());
	process_step(5, 6, UFid());
	process_step(0, 2, UFid());
	process_step(6, 1, UFid());
	printf("\n");
	process_step(5, 8, UFid());

	return 0;
}
