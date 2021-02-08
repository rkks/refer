/* Program 1.2: Quick-union solution to connectivity problem
 * This program does less computation for the *union* operation
 * at the expense of more computation for the *find* operation.
 * The *for* loops and subsequent *if* statement in this code
 * specify the necessary and sufficient conditions on the id array
 * for p and q to be connected. The assignment statement id[i] = j
 * implements the union operation.
 *
 * Property 1.2: For M > N, the quick-union algorithm could take
 * more than MN/2 instructions to solve a connectivity problem
 * with M pairs of N objects.
 */
#include <stdio.h>
#define N 10000

int main() {
	int i, j, p, q, id[N];

	for (i = 0; i < N; i++)
		id[i] = i;
	while (scanf("%d %d\n", &p, &q) == 2) {
		for (i = p; i != id[i]; i = id[i])
	    ;
		for (j = q; j != id[j]; j = id[j])
	    ;
		if (i == j)
	    continue;
		id[i] = j;
		printf(" %d %d\n", p, q);
	}
	return 0;
}
