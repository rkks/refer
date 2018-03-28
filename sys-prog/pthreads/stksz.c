/*
 * Stack size test: stksz [size-in-mb]
 * Benutzt pmap um Stacksegment zu zeigen.
 * Rekursiver Aufruf bis SEGV.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void *Thread(void *);
void Error(const char *str, ...);

void
f(int n)
{
	static char *p = NULL;
	char v[10 * 1024];

	if (!p) 
		p = v;
	
	printf("*** n = %d, stack size %d\n", n, p - v);

	if (n % 20 == 0) {
		printf("\npmap:");
		sprintf(v, "pmap %d", getpid());
		system(v);
	}
	f(n + 1);
}

void *
Thread(void *v)
{
	char buf[80];

	printf("\npmap nach pthread_create:\n");
	sprintf(buf, "pmap %d", getpid());
	system(buf);

	f(0);
	return NULL;
}

int
main(int argc, char **argv)
{
	int e;
	pthread_t t;
	pthread_attr_t attr;
	char buf[80];

	if (pthread_attr_init(&attr))
		Error("pthread_attr_init");

	if ((e = pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM)))
		Error("pthread_attr_setscope %d", e);

	if (argc > 1) {
		size_t s;
		s = atoi(argv[1]) * 1024 * 1024;
		if ((e = pthread_attr_setstacksize(&attr, s)))
			Error("pthread_attr_setscope %d", e);
	}

	printf("pmap vor pthread_create:\n");
	sprintf(buf, "pmap %d", getpid());
	system(buf);

     	if (e = pthread_create(&t, &attr, Thread, NULL))
		Error("pthread_create %d", e);

	/* f(0); */
	if (pthread_join(t, NULL))
		Error("pthread_join");

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
