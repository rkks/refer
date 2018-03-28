/*
 * Fehlerhaftes Programm: Erzeugt 5 Threads und beendet main. Ob einer
 * der erzuegten Threads ueberhaupt anfaengt zu laufen ist
 * implementierungsabhaengig.
 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "wrapper.h"

struct thread_data {
	int number;
};

void *Thread(void *);

void *
Thread(void *v)
{
	printf("Thread start\n");
	sleep(4);
	printf("Thread exit\n");

	return NULL;
}

int
main(int argc, char **argv)
{
	int n;
	pthread_t t;

	for (n = 0; n < 5; n++) {
		printf("created thread %d\n", n);
		Pthread_create(&t, NULL, Thread, NULL);
	}
}
