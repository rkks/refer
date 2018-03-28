#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/sem.h>
#include "ssp.h"

union semun {
	int val;
	struct semid_ds *buf;
	ushort_t *array;
};

static void print_sem_set (int semid);

int main (int argc, char **argv)
{
	int c;
	int err_flag;
	int semid;
	int *buf;
	uint_t i;
	uint_t n;
	uint_t nids;

	nids = 0;
	buf = NULL;
	semid = 0;
	opterr = 0;
	err_flag = 0;

	while ((c = getopt (argc, argv, "s:")) != EOF) {
		switch (c) {
			case 's':
				semid = atoi (optarg);
				break;

			default:
				err_flag++;
				break;
		}
	}

	if (optind != argc)
		err_flag++;
	if (err_flag)
		err_quit ("Usage: semls [-s semaphore set ID]");

	if (semid != 0)
		print_sem_set (semid);
	else {
		for (;;) {
			if (semids (buf, nids, &n) == -1)
				err_msg ("semids failed");

			if (n <= nids)
				break;

			nids = n;
			if ((buf = realloc (buf, nids * sizeof (int))) == NULL)
				err_msg ("realloc failed");
		}

		for (i = 0; i < n; i++)
			print_sem_set (buf [i]);

		free (buf);
	}

	return (0);
}

static void print_sem_set (int semid)
{
	int num_sems;
	struct semid_ds sem_info;
	union semun arg;
	unsigned short *buf;
	int i;

	arg.buf = &sem_info;
	if (semctl (semid, 0, IPC_STAT, arg) == -1)
		err_msg ("semctl failed");
	num_sems = arg.buf -> sem_nsems;

	if ((buf = calloc (num_sems, sizeof (unsigned short))) == NULL)
		err_msg ("calloc failed");
	arg.array = buf;

	if (semctl (semid, 0, GETALL, arg) == -1)
		err_msg ("semctl failed");

	printf ("Semaphore set ID %d:\n", semid);
	for (i = 0; i < num_sems; i++)
		printf ("  semaphore %d = %d\n", i, buf [i]);

	free (buf);
}
