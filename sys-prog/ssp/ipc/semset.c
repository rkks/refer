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

int main (int argc, char **argv)
{
	int semid;
	int num_sems;
	key_t key;
	struct semid_ds sem_info;
	union semun arg;
	unsigned short *buf;
	int i;

	if (argc < 2)
		err_quit ("Usage: semset path [values ...]");

	if ((key = ftok (argv [1], 0x42)) == -1)
		err_msg ("ftok failed");
	if ((semid = semget (key, 0, 0)) == -1)
		err_msg ("semget failed");

	arg.buf = &sem_info;
	if (semctl (semid, 0, IPC_STAT, arg) == -1)
		err_msg ("semctl (IPC_STAT) failed");
	num_sems = arg.buf -> sem_nsems;
	if (argc != num_sems + 2)
		err_quit ("%d semaphores in set, %d values specified", num_sems,
		argc - 2);

	if ((buf = calloc (num_sems, sizeof (unsigned short))) == NULL)
		err_msg ("calloc failed");
	arg.array = buf;
	for (i = 0; i < num_sems; i++)
		buf [i] = atoi (argv [i + 2]);
	if (semctl (semid, 0, SETALL, arg) == -1)
		err_msg ("semctl (SETALL) failed");

	return (0);
}
