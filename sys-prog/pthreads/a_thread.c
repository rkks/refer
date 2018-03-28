/*
 * Erzeugen eines Threads
 */

#include <pthread.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

struct thread_data {
	const char *name;
};

void *Thread(void *);
void Error(const char *str, ...);

void *
Thread(void *v)
{
	struct thread_data *p = (struct thread_data *)v;

	printf("ich bin thread %s\n", p->name);
	p->name = "HUGO";

	sleep(4);

	return v;
}

int
main(int argc, char **argv)
{
	int e;
	pthread_t t;
	pthread_attr_t attr;
	struct thread_data d, *res;

	if (pthread_attr_init(&attr))
		Error("pthread_attr_init");

	if ((e = pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM)))
		Error("pthread_attr_setscope %d", e);

	d.name = "hugo";
     	if (e = pthread_create(&t, &attr, Thread, (void *)&d))
		Error("pthread_create %d", e);

	sleep(1);
	system("ps -fL");

	if (pthread_join(t, (void **)&res))
		Error("pthread_join");

	printf("das war thread %s\n", res->name);

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
