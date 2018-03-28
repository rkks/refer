#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define RECORD_FILE "records.dat"

FILE *fp;

static void *write_record (void *arg);

int main (void)
{
	pthread_t thread1;
	pthread_t thread2;

	if ((fp = fopen (RECORD_FILE, "w")) == NULL) {
		perror ("Can't open " RECORD_FILE);
		exit (1);
	}

	pthread_setconcurrency (2);
	pthread_create (&thread1, NULL, write_record, NULL);
	pthread_create (&thread2, NULL, write_record, NULL);

	pthread_join (thread1, NULL);
	pthread_join (thread2, NULL);

	fclose (fp);

	return (0);
}

static void *write_record (void *arg)
{
	pthread_t thread_id;

	thread_id = pthread_self ();

	flockfile (fp);
	fprintf (fp, "%u: Name\n", thread_id);
	fprintf (fp, "%u: Address\n", thread_id);
	fprintf (fp, "%u: City\n", thread_id);
	fprintf (fp, "%u: Province\n", thread_id);
	fprintf (fp, "%u: Post Code\n", thread_id);
	fprintf (fp, "%u: Phone\n", thread_id);
	fprintf (fp, "%u: Fax\n", thread_id);
	fprintf (fp, "%u: Email\n", thread_id);
	funlockfile (fp);

	return (NULL);
}
