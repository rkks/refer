#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	int c;
	int shmid;
	size_t size;
	key_t key;
	int shmflg;
	int err_flag;

	shmflg = SHM_R | SHM_W | (SHM_R >> 3) | IPC_CREAT;
	opterr = 0;
	err_flag = 0;

	while ((c = getopt (argc, argv, "e")) != EOF) {
		switch (c) {
			case 'e':
				shmflg |= IPC_EXCL;
				break;

			default:
				err_flag++;
				break;
		}
	}

	if (optind != argc - 2)
		err_flag++;

	if (err_flag)
		err_quit ("Usage: shmcreat [-e] path size");

	size = atoi (argv [optind + 1]);
	if ((key = ftok (argv [optind], 0x42)) == -1)
		err_msg ("ftok failed");
	if ((shmid = shmget (key, size, shmflg)) == -1)
		err_msg ("shmget failed");

	printf ("Shared memory ID = %d\n", shmid);

	return (0);
}
