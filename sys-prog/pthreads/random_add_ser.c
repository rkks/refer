/* 
 * Das Programm berechnet NSUMS Summen aus je N Randomzahlen und ermittelt
 * am Ende die Gesamtsumme. Die Aufgabenstellung eignet sich gut zur
 * Parallelisierung in NSUMS Threads.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "wrapper.h"

#define N 10000000
#define NSUMS 4

unsigned results[NSUMS];

int
main(int argc, char **argv)
{
	int n, k;
	unsigned sum;
	unsigned seed;

	for (n = 0; n < NSUMS; ++n) {

		/* berechne results[n], n-te Summe von N Ramdomzahlen */
		seed = n;
		for (k = 0; k < N; ++k)
			results[n] += rand_r(&seed);
	}

	/* Berechne Summe der Summen */
	sum = 0;
	for (n = 0; n < NSUMS; ++n)
		sum += results[n];

	printf("sum %u\n", sum);

	return 0;
}
