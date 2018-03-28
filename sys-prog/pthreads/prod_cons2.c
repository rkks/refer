/*
 * Producer / Consumer Problem, verbesserte Version
 *
 * Aufruf: prod_cons num_producers num_consumers num_items buflen busy-loops
 */

#include <pthread.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <stdlib.h>
#include <strings.h>
#include "wrapper.h"

int buflen = 1000;

/* Packets */
struct packet {
	int owner;
	struct packet *next;
	int len;
	unsigned buf[1];	/* variable Laenge, = buflen */
};

/* 
 * Producers 
 */

struct producer {
	pthread_t tid;
	int no;
	unsigned seed;
	unsigned sum;
	pthread_mutex_t queue_mutex;
	pthread_cond_t  empty_cond;
	struct packet *emptyPacketQueue;
};

#define MAXPRODUCERS 20
struct producer producers[MAXPRODUCERS];
int num_producers = 4;

/*
 * Consumers
 */

struct consumer {
	pthread_t tid;
	int no;
	unsigned sum;
	int eof;
	pthread_mutex_t queue_mutex;
	pthread_cond_t  empty_cond;
	struct packet *fullPacketQueue;
};

#define MAXCONSUMERS 20
struct consumer consumers[MAXCONSUMERS];

int num_consumers = 4;

int num_items = 10000;
int busy_loops = 1000;

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
	struct producer *p = (struct producer *)v;
	struct consumer *c;
	struct packet *pa, *old;
	unsigned seed = p->seed;
	int n, k, cons;
	unsigned sum = 0;
	int waits = 0;
	int more = 1;
	int item_cnt = 0;

	cons = seed % num_consumers;
	for (n = 0; more; ++n) {

		/* Hole Packet aus lokaler emptyPacketQueue */
		Pthread_mutex_lock(&p->queue_mutex);
		while (!p->emptyPacketQueue) {
			++waits;
			Pthread_cond_wait(&p->empty_cond, &p->queue_mutex);
		}
		pa = p->emptyPacketQueue;
		p->emptyPacketQueue = pa->next;
		Pthread_mutex_unlock(&p->queue_mutex);

		/* Fuelle Packet */
		for (k = 0; k < buflen; ++k) {
			pa->buf[k] = ProduceItem(&seed, &sum);
			if (++item_cnt == num_items) {
				more = 0;
				++k;
				break;
			}
		}
		pa->len = k;

		/* Versende Packet an Consumer cons */
		c = consumers + cons;
		Pthread_mutex_lock(&c->queue_mutex);
		old = pa->next = c->fullPacketQueue;
		c->fullPacketQueue = pa;
		Pthread_mutex_unlock(&c->queue_mutex);
		if (!old)
			Pthread_cond_broadcast(&c->empty_cond);

		if (++cons == num_consumers)
			cons = 0;
	}

	p->sum = sum;
	printf("Producer %d exit, %d waits, %d packets\n", p->no, waits, n);
	return NULL;
}

void *
Consumer(void *v)
{
	struct consumer *c = (struct consumer *)v;
	struct producer *p;
	struct packet *pa, *old;
	unsigned sum = 0;
	int n, k;
	int waits = 0;
	int item_cnt = 0;
	
	for (n = 0; ; ++n) {
		/* Hole Packet aus lokaler fullPacketQueue */
		Pthread_mutex_lock(&c->queue_mutex);
		while (!c->fullPacketQueue) {
			if (c->eof) {
				Pthread_mutex_unlock(&c->queue_mutex);
				goto done;
			}
			++waits;
			Pthread_cond_wait(&c->empty_cond, &c->queue_mutex);
		}
		pa = c->fullPacketQueue;
		c->fullPacketQueue = pa->next;
		Pthread_mutex_unlock(&c->queue_mutex);

		/* Verarbeite Packet */
		for (k = 0; k < pa->len; ++k)
			ConsumeItem(pa->buf[k], &sum);
		item_cnt += pa->len;
		
		/* Stelle Packet zurueck in emptyPacketQueue des Owners */
		p = producers + pa->owner;
		Pthread_mutex_lock(&p->queue_mutex);
		old = pa->next = p->emptyPacketQueue;
		p->emptyPacketQueue = pa;
		Pthread_mutex_unlock(&p->queue_mutex);
		if (!old)
			Pthread_cond_broadcast(&p->empty_cond);
	}

done:
	c->sum = sum;
	printf("Consumer %d exit, %d items in %d packets received, %d waits\n", 
		c->no, item_cnt, n, waits);
	return NULL;
}

int
main(int argc, char **argv)
{
	int n, k, e;
	unsigned prod_sum, cons_sum;
	pthread_attr_t attr;
	struct packet *p;
	int packets_per_producer;

 	if (argc > 1 && !strcmp("-", argv[1]))
 		Error("usage: %s num_producers num_consumers "
			"num_items buflen busy-loops", argv[0]);

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
		buflen = atoi(argv[4]);
		if (buflen < 1)
			buflen = 1;
	}
	if (argc > 5)
		busy_loops = atoi(argv[5]);
	printf("num_producers %d num_consumers %d num_items %d "
		"buflen %d busy-loops %d\n",
		num_producers, num_consumers, num_items, buflen, busy_loops);
	num_items /= num_producers; /* items/producer */

	Pthread_attr_init(&attr);

	Pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

	/* Producer Datenstruktur */
	packets_per_producer = (num_producers + num_consumers) * 2;
	for (n = 0; n < num_producers; ++n) {
		Pthread_mutex_init(&producers[n].queue_mutex, NULL);
		Pthread_cond_init(&producers[n].empty_cond, NULL);
		producers[n].seed = n;
		producers[n].no = n;
		producers[n].emptyPacketQueue = NULL;
		for (k = 0; k < packets_per_producer; ++k) {
			p = malloc(sizeof(struct packet) + 
				(buflen - 1) * sizeof(unsigned));
			if (!p)
				Error("malloc");

			p->owner = n;
			p->next = producers[n].emptyPacketQueue;
			producers[n].emptyPacketQueue = p;
		}
	}

	/* Consumer Datenstruktur */
	for (n = 0; n < num_consumers; ++n) {
		Pthread_mutex_init(&consumers[n].queue_mutex, NULL);
		Pthread_cond_init(&consumers[n].empty_cond, NULL);
		consumers[n].no = n;
		consumers[n].eof = 0;
		consumers[n].fullPacketQueue = NULL;
	}

	/* Starte die Producer */
	for (n = 0; n < num_producers; ++n)
     		Pthread_create(&producers[n].tid, &attr, 
						Producer, &producers[n]);

	/* Starte die Consumer */
	for (n = 0; n < num_consumers; ++n)
     		Pthread_create(&consumers[n].tid, &attr, 
						Consumer, &consumers[n]);

	/* Warte auf alle Producer */
	prod_sum = 0;
	for (n = 0; n < num_producers; ++n) {
		Pthread_join(producers[n].tid, NULL);

		prod_sum += producers[n].sum;
	}

	/* setze eof und wecke consumer auf */
	for (n = 0; n < num_consumers; ++n) {
		Pthread_mutex_lock(&consumers[n].queue_mutex);
		consumers[n].eof = 1;
		Pthread_mutex_unlock(&consumers[n].queue_mutex);
		Pthread_cond_broadcast(&consumers[n].empty_cond);
	}

	/* Warte auf alle Consumer */
	cons_sum = 0;
	for (n = 0; n < num_consumers; ++n) {
		Pthread_join(consumers[n].tid, NULL);

		cons_sum += consumers[n].sum;
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
