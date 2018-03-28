#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	int shmid;
	key_t key;

	if (argc != 2)
		err_quit ("Usage: shmrm path");

	if ((key = ftok (argv [1], 0x42)) == -1)
		err_msg ("ftok failed");
	if ((shmid = shmget (key, 0, 0)) == -1)
		err_msg ("shmget failed");

	if (shmctl (shmid, IPC_RMID, NULL) == -1)
		err_msg ("shmctl failed");

	return (0);
}
