#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/sem.h>
#include "ssp.h"

#define NUM_ITERS 50000

union semun {
	int val;
	struct semid_ds *buf;
	ushort *array;
};

static void do_ops (int semid);

int main (int argc, char **argv)
{
	pid_t pid;
	int semid;
	union semun arg;

	if (argc != 1)
		err_quit ("Usage: semperf");

	if ((semid = semget (IPC_PRIVATE, 1, SEM_R | SEM_A | IPC_CREAT)) == -1)
		err_msg ("semget failed");

	arg.val = 1;
	if (semctl (semid, 0, SETVAL, arg) == -1)
		err_msg ("semctl failed");

	switch (pid = fork ()) {
		case -1:
			err_msg ("fork failed");

		case 0:
			do_ops (semid);
			_exit (0);

		default:
			do_ops (semid);
			waitpid (pid, NULL, 0);
			break;
	}

	semctl (semid, IPC_RMID, NULL);

	return (0);
}

static void do_ops (int semid)
{
	struct sembuf buf [2];
	int i;

	for (i = 0; i < NUM_ITERS; i++) {
		buf [0].sem_num = 0;
		buf [0].sem_op = -1;
		buf [0].sem_flg = SEM_UNDO;
		buf [1].sem_num = 0;
		buf [1].sem_op = 1;
		buf [1].sem_flg = SEM_UNDO;

		if (semop (semid, buf, 2) == -1)
			err_msg ("semop failed");
	}
}
