/*
 * kein Producer / Consumer Problem, single threaded, vgl. prod_cons.c
 *
 * Aufruf: no_prod_cons num_items busy-loops
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

int num_items = 10000;
int busy_loops = 1000;

int ProduceItem(unsigned *seed, unsigned *sum);
void ConsumeItem(int item, unsigned *sum);

int
ProduceItem(unsigned *seed, unsigned *sum)
{
	int item, k;

	item = rand_r(seed);
	*sum += item;

	/* simuliere Verarbeitung */
	for (k = 0; k < busy_loops; ++k)
		;

	return item;
}

void
ConsumeItem(int item, unsigned *sum)
{
	int k;
	
	*sum += item;

	/* simuliere Verarbeitung */
	for (k = 0; k < busy_loops; ++k)
		;
}

int
main(int argc, char **argv)
{
	int n, e, item;
	unsigned seed, prod_sum, cons_sum;

 	if (argc > 1 && !strcmp("-", argv[1])) {
 		printf("usage: %s num_items busy_loops\n", argv[0]);
		exit(1);
	}

	if (argc > 1) {
		num_items = atoi(argv[1]);
		if (num_items < 0)
			num_items = 0;
	}
	if (argc > 2)
		busy_loops = atoi(argv[2]);
	printf("num_items %d busy-loops %d\n", num_items, busy_loops);

	seed = prod_sum = cons_sum = 0;
	for (n = 0; n < num_items; ++n) {
		item = ProduceItem(&seed, &prod_sum);
		ConsumeItem(item, &cons_sum);
	}

	printf("prot_sum %u cons_sum %u\n", prod_sum, cons_sum);
	if (cons_sum != prod_sum)
		printf("ERROR\n");

	return 0;
}
