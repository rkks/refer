#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	int i;
	int shmid;
	key_t key;
	unsigned char c;
	char *buf;
	struct shmid_ds shm_info;

	if (argc != 2)
		err_quit ("Usage: shmread path");

	if ((key = ftok (argv [1], 0x42)) == -1)
		err_msg ("ftok failed");
	if ((shmid = shmget (key, 0, 0)) == -1)
		err_msg ("shmget failed");

	if ((buf = shmat (shmid, NULL, 0)) == (char *) -1)
		err_msg ("shmat failed");

	if (shmctl (shmid, IPC_STAT, &shm_info) == -1) {
		shmdt (buf);
		err_msg ("shmctl failed");
	}

	for (i = 0; i < shm_info.shm_segsz; i++)
		if ((c = *buf++) != (i % 256))
			printf ("buf [%d] = %d (should be %d)\n", i, c, i % 256);

	shmdt (buf);

	return (0);
}
