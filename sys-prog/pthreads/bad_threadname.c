/* 
 * Fehlerhaftes Programm zum Debuggen. Die Funktion threadname() ist nicht
 * threadsave (und ausserdem unportable). Der Fehler tritt auf 
 * Singleprozessorsystemen kaum je auf, auf Multiprozessoren aber wohl.
 *
 * Mit einem Debugger kann man ihn leicht demonstrieren: Man stoppt beide
 * Threads bei "return buf;" am Ende von threadname() und laesst sie dann
 * weiterlaufen.
 *
 * mit dbx:
 *
 *	dbx bad_threadname
 *	(dbx) stop at bad_threadname.c:97
 *	(dbx) run
 *	t@5 (l@5) stopped in threadname at line 97 in file "bad_threadname.c"
 *	(dbx) threads
 *	*>    t@5  b l@5  Thread()      breakpoint    in threadname()
 * 	      t@6  b l@6  Thread()      running       in __lwp_schedctl()
 *	(dbx) cont t@6
 *	t@6 (l@6) stopped in threadname at line 97 in file "bad_threadname.c"
 *	(dbx) cont
 *	ich bin thread 5, t@6
 *	ich bin thread 6, t@6
 *
 * Falls dbx bei "cont t@6" haengt oder Fehlermeldungen bringt:
 *	
 * 	Das ist offenbar ein dbx bug. Scheint damit zusammenzuhaengen, dass
 * 	t@6 noch nicht ganz fertig erzeugt ist.
 *
 *		t@6  b l@6  Thread()      suspended       in _thread_start()
 *
 *	Man benutze "cont" bis beide Threads laufen. Z.B.:
 *
 *	(dbx)stop at bad_threadname.c:97
 *	(dbx)run
 *	t@5 (l@5) stopped in threadname at line 97 in file "bad_threadname.c"
 *	(dbx) cont
 *	ich bin thread 5, t@5
 *	t@6 (l@6) stopped in threadname at line 97 in file "bad_threadname.c"
 *	(dbx) cont t@5
 *	t@5 (l@5) stopped in threadname at line 97 in file "bad_threadname.c"
 *	(dbx) cont
 *	ich bin thread 6, t@5
 *	ich bin thread 5, t@5
 */

#include <pthread.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


void *Thread(void *);
char *threadname();
void Error(char *str, ...);

int
main(int argc, char **argv)
{
	int n, e, scope;
	pthread_t t;
	pthread_attr_t attr;

	if (pthread_attr_init(&attr))
		Error("pthread_attr_init");
	if ((e = pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM)))
		Error("pthread_attr_setscope %d", e);

	for (n = 0; n < 2; ++n)
     		if (e = pthread_create(&t, &attr, Thread, NULL))
			Error("pthread_create %d", e);

	pthread_exit(NULL);
}

void *
Thread(void *arg)
{
	int n;
	int num = pthread_self();

	for (n = 0; n < 10; ++n) {
		printf("ich bin thread %d, %s\n", num, threadname());
		sleep(1);
	}
}

char *
threadname()
{
	static char buf[40];

	sprintf(buf, "t@%d", pthread_self());

	return buf;
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
