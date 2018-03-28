/*
 * Aufgabe: Erzeugen von 5 Threads mit je ca. 5 Sekunden Rechenzeit,
 *          mit und ohne PTHREAD_SCOPE_SYSTEM.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#define NTHREADS 5

struct thread_data {
	int my_id;
	pthread_t tid;
	int status;
};

struct thread_data thread_data[NTHREADS];

void *Thread(void *);
void Error(const char *str, ...);

void *
Thread(void *v)
{
	struct thread_data *p = (struct thread_data *)v;
	int n;

	printf("start thread %d\n", p->my_id);
	for (n = 0; n < 1000000000; ++n)
		;
	p->status = p->my_id + 1;
	printf("ende thread %d\n", p->my_id);

	return v;
}

int
main(int argc, char **argv)
{
	int e, n;
	pthread_attr_t attr;
	struct thread_data *res;

	if (pthread_attr_init(&attr))
		Error("pthread_attr_init");

	/*
	if ((e = pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM)))
		Error("pthread_attr_setscope %d", e);
	*/

	for (n = 0; n < NTHREADS; ++n) {
		thread_data[n].my_id = n;
     		if (e = pthread_create(&thread_data[n].tid, &attr, Thread, 
						(void *)&thread_data[n]))
			Error("pthread_create %d", e);
	}

	for (n = 0; n < NTHREADS; ++n) {
		if (pthread_join(thread_data[n].tid, (void **)&res))
			Error("pthread_join");
		printf("das war thread %d, status %d\n", 
			res->my_id, res->status);
	}

	return 0;
}

void
Error(const char *str, ...)
{
        va_list ap;
	int e;

	e = errno;
        va_start(ap, str);
        vfprintf(stderr, str, ap);
	va_end(ap);
        fprintf( stderr, "\n" );
	if (e) {
		errno = e;
		perror("system error");
		fprintf( stderr, "errno %d\n", errno);
	}

        exit(1);
}
