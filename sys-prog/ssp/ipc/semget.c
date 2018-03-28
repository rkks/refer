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

	if (argc != 2)
		err_quit ("Usage: semget path");

	if ((key = ftok (argv [1], 0x42)) == -1)
		err_msg ("ftok failed");
	if ((semid = semget (key, 0, 0)) == -1)
		err_msg ("semget failed");

	arg.buf = &sem_info;
	if (semctl (semid, 0, IPC_STAT, arg) == -1)
		err_msg ("semctl failed");
	num_sems = arg.buf -> sem_nsems;

	if ((buf = calloc (num_sems, sizeof (unsigned short))) == NULL)
		err_msg ("calloc failed");
	arg.array = buf;

	if (semctl (semid, 0, GETALL, arg) == -1)
		err_msg ("semctl failed");
	for (i = 0; i < num_sems; i++)
		printf ("semval [%d] = %d\n", i, buf [i]);

	return (0);
}
