#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	int semid;
	key_t key;

	if (argc != 2)
		err_quit ("Usage: semrm path");

	if ((key = ftok (argv [1], 0x42)) == -1)
		err_msg ("ftok failed");
	if ((semid = semget (key, 0, 0)) == -1)
		err_msg ("semget failed");

	if (semctl (semid, IPC_RMID, NULL) == -1)
		err_msg ("semctl failed");

	return (0);
}
