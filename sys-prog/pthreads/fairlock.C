/* Faires Lock mit C++ Klassen.
 */

#include <stdio.h>
#include <pthread.h>
#include <stdio.h>
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>
#include "wrapper.h"

extern "C" {
typedef void *VOID_PTR;
typedef  VOID_PTR THREAD_START_FUNC(VOID_PTR);
}

class MyThread {
	pthread_t me;
	pthread_cond_t c;
	MyThread *next;

	void wakeup() {
		Pthread_cond_broadcast(&c);
	}

	void wait(pthread_mutex_t *mutex) {
		Pthread_cond_wait(&c, mutex);
	}

	void setNext(MyThread *n) { next = n; }
	MyThread *getNext() { return next; }

	friend class FairLock;
    public:

	MyThread(THREAD_START_FUNC t) {
		Pthread_cond_init(&c, NULL);
		Pthread_create(&me, NULL, t, (void *) this);
	}
	~MyThread() {
		Pthread_cond_destroy(&c);
	}

	void print(const char *s) {
		char b[500];
		int l = sprintf(b, "thread %02d: %s\n", me, s);
		write(1, b, l);
	}
};

class FairLock {
	pthread_mutex_t m;
	MyThread *q;
	MyThread *qEnd;
	int locked;
    public:
	FairLock() {
		Pthread_mutex_init(&m, NULL);
		locked = 0;
		q = qEnd = 0;
	}

	~FairLock() {
		Pthread_mutex_destroy(&m);
	}

	void lock(MyThread *me) {
		Pthread_mutex_lock(&m);
		if (locked || q) {
			if (q)
				qEnd->setNext(me);
			else
				q = me;
			me->setNext(0);
			qEnd = me;

			while (locked || q != me)
				me->wait(&m);

			q = q->getNext();
			if (!q)
				qEnd = 0;
		}
		locked = 1;
		Pthread_mutex_unlock(&m);
	}

	void unlock() {
		MyThread *w = 0;

		Pthread_mutex_lock(&m);
		locked = 0;
		w = q;
		Pthread_mutex_unlock(&m);

		if (w)
			w->wakeup();
	}
};

FairLock fl;

extern "C" void *
Thread(void *arg)
{
	MyThread *me = (MyThread *)arg;
	int n;
	unsigned int t, s = (int)arg;
	char b[100];
	time_t t1, t2;

	me->print("start");
	for (n = 1; ;++n) {
		t = rand_r(&s) % 4;

		t1 = time(NULL);
		fl.lock(me);
		t2 = time(NULL);
		sprintf(b, "lock %03d at %d, start wait at %d", n, t2, t1);
		me->print(b);
		if (t)
			sleep(t);
		fl.unlock();

		t = rand_r(&s) % 4;
		if (t)
			sleep(t);
	}

	return 0;
}

int
main()
{

	for (int n = 1; n < 20; ++n)
		new MyThread(Thread);

	pthread_exit(0);
	return 0;
}
