/*
 * Producer / Consumer Problem
 *
 * Aufruf: prod_cons num_producers num_consumers num_items maxbuflen busy-loops
 */

#include <pthread.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <stdlib.h>
#include <strings.h>
#include "wrapper.h"

/* producer / consumer buffer */
#define MAXBUFLEN 100000
unsigned buf[MAXBUFLEN];
int maxbuflen = 100;
int buflen = 0;
int eof = 0;

/* buffer mutex */
pthread_mutex_t buf_mutex = PTHREAD_MUTEX_INITIALIZER;

/* condition buf voll: buflen == maxbuflen. Mutex: buf_mutex */
pthread_cond_t  full_cond  = PTHREAD_COND_INITIALIZER;

/* condition buf leer oder eof: buflen <= 0. Mutex: buf_mutex */
pthread_cond_t  empty_cond  = PTHREAD_COND_INITIALIZER;

#define MAXPRODUCERS 100
pthread_t producers[MAXPRODUCERS];
int num_producers = 4;

#define MAXCONSUMERS 100
pthread_t consumers[MAXCONSUMERS];
int num_consumers = 4;

int num_items = 10000;
int busy_loops = 1000;

struct thread_data {
	int no;
	unsigned seed;
	unsigned sum;
};

void *Producer(void *);
int ProduceItem(unsigned *seed, unsigned *sum);
void *Consumer(void *);
void ConsumeItem(int item, unsigned *sum);
void Error(char *str, ...);

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

void *
Producer(void *v)
{
	struct thread_data *data = (struct thread_data *)v;
	unsigned seed = data->seed;
	int n;
	unsigned item, sum = 0;
	int waits = 0;

	for (n = 0; n < num_items; ++n) {
		item = ProduceItem(&seed, &sum);
	
		Pthread_mutex_lock(&buf_mutex);
		while (buflen == maxbuflen) {
			++waits;
			Pthread_cond_wait(&full_cond, &buf_mutex);
		}
		buf[buflen++] = item;
		if (buflen == 1)
			Pthread_cond_broadcast(&empty_cond);
		Pthread_mutex_unlock(&buf_mutex);
	}

	data->sum = sum;
	printf("Producer %d exit, %d waits\n", data->no, waits);
	return data;
}

void *
Consumer(void *v)
{
	struct thread_data *data = (struct thread_data *)v;
	unsigned sum = 0;
	int n, item;
	int waits = 0;
	int l = maxbuflen - 1;
	
	for (n = 0; ; ++n) {
		Pthread_mutex_lock(&buf_mutex);
		while (buflen == 0) {
			if (eof) {
				Pthread_mutex_unlock(&buf_mutex);
				goto done;
			}
			++waits;
			Pthread_cond_wait(&empty_cond, &buf_mutex);
		}
		item = buf[--buflen];
		if (buflen == l)
			Pthread_cond_broadcast(&full_cond);
		Pthread_mutex_unlock(&buf_mutex);

		ConsumeItem(item, &sum);
	}

done:
	data->sum = sum;
	printf("Consumer %d exit, %d items received, %d waits\n", 
		data->no, n, waits);
	return data;
}

int
main(int argc, char **argv)
{
	int n, e;
	unsigned prod_sum, cons_sum;
	struct thread_data *data;
	pthread_attr_t attr;

 	if (argc > 1 && !strcmp("-", argv[1]))
 		Error("usage: %s num_producers num_consumers "
			"num_items maxbuflen busy-loops", argv[0]);

	if (argc > 1) {
		num_producers = atoi(argv[1]);
		if (num_producers < 0)
			num_producers = 1;
		if (num_producers > MAXPRODUCERS)
			num_producers = MAXPRODUCERS;
	}
	if (argc > 2) {
		num_consumers = atoi(argv[2]);
		if (num_consumers < 0)
			num_consumers = 1;
		if (num_consumers > MAXCONSUMERS)
			num_consumers = MAXCONSUMERS;
	}
	if (argc > 3) {
		num_items = atoi(argv[3]);
		if (num_items < 0)
			num_items = 0;
	}
	if (argc > 4) {
		maxbuflen = atoi(argv[4]);
		if (maxbuflen < 0)
			maxbuflen = 1;
		if (maxbuflen > MAXBUFLEN)
			maxbuflen = MAXBUFLEN;
	}
	if (argc > 5)
		busy_loops = atoi(argv[5]);
	printf("num_producers %d num_consumers %d num_items %d "
		"maxbuflen %d busy-loops %d\n",
		num_producers, num_consumers, num_items, maxbuflen, busy_loops);
	num_items /= num_producers; /* items/producer */

	Pthread_attr_init(&attr);

	Pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

	/* Starte die Producer */
	for (n = 0; n < num_producers; ++n) {
		data = malloc(sizeof(struct thread_data));
		if (!data)
			Error("malloc");
		data->seed = n;
		data->no = n;
     		Pthread_create(&producers[n], &attr, Producer, data);
	}

	/* Starte die Consumer */
	for (n = 0; n < num_consumers; ++n) {
		data = malloc(sizeof(struct thread_data));
		if (!data)
			Error("malloc");
		data->no = n;
     		Pthread_create(&consumers[n], &attr, Consumer, data);
	}

	/* Warte auf alle Producer */
	prod_sum = 0;
	for (n = 0; n < num_producers; ++n) {
		Pthread_join(producers[n], (void **)&data);

		prod_sum += data->sum;
	}

	/* setze eof und wecke consumer auf */
	Pthread_mutex_lock(&buf_mutex);
	eof = 1;
	Pthread_mutex_unlock(&buf_mutex);
	Pthread_cond_broadcast(&empty_cond);

	/* Warte auf alle Consumer */
	cons_sum = 0;
	for (n = 0; n < num_consumers; ++n) {
		Pthread_join(consumers[n], (void **)&data);

		cons_sum += data->sum;
	}

	printf("prot_sum %u cons_sum %u\n", prod_sum, cons_sum);
	if (cons_sum != prod_sum)
		printf("ERROR\n");

	return 0;
}

void
Error(char *str, ...)
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
