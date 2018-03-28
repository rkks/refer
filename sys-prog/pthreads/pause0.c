/* 
 * Das Programm erzeugt 4 Threads, die in einer Schleife ihren Namen
 * ausgeben. Alle 5 Sekunden machen sie eine 5 Sekunden lange Pause.
 *
 * Das Pause machen soll unter Verwendung einer condition umgeschrieben
 * werden.
 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <poll.h>
#include "wrapper.h"

int pause_flag = 0;

void *Thread(void *);
char *threadname();

int
main(int argc, char **argv)
{
	int n, scope;
	pthread_t t;
	pthread_attr_t attr;

	Pthread_attr_init(&attr);
	Pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

	for (n = 0; n < 4; ++n)
     		Pthread_create(&t, &attr, Thread, (void *)n);

	for (;;) {
		sleep(5);

		pause_flag = 1;
		printf("Pause!!\n");

		sleep(5);

		printf("Weiter!!\n");
		pause_flag = 0;

	}
}

void *
Thread(void *arg)
{
	int num = (int)arg;

	for (;;) {
		while (pause_flag)
			 poll(0, NULL, 100);

		printf("ich bin thread %d\n", num);
		poll(0, NULL, 100);
	}
}
