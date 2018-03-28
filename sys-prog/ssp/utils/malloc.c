#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include "ssp.h"

#define NUM_CHUNKS 10000
#define CHUNK_SIZE1 1152
#define CHUNK_SIZE2 1500
#define NUM_ITERS 100

int main (void)
{
	int i;
	int j;
	void *ptrs [NUM_CHUNKS];
	time_t start;
	time_t fin;

	start = time (NULL);

	for (i = 0; i < NUM_ITERS; i++) {
		printf ("Iteration = %d\n", i);
		for (j = 0; j < NUM_CHUNKS; j++) {
			if ((ptrs [j] = malloc (CHUNK_SIZE1)) == NULL)
				err_msg ("malloc fails");
		}

		for (j = 0; j < NUM_CHUNKS; j++)
			free (ptrs [j]);

		for (j = 0; j < NUM_CHUNKS; j++) {
			if ((ptrs [j] = malloc (CHUNK_SIZE2)) == NULL)
				err_msg ("malloc fails");
		}

		for (j = 0; j < NUM_CHUNKS; j++)
			free (ptrs [j]);
	}

	fin = time (NULL);

	printf ("Run time = %ld seconds\n", fin - start);
	pause ();

	return (0);
}
