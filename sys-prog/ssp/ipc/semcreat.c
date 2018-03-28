#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/sem.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	int c;
	int semid;
	int num_sems;
	key_t key;
	int semflg;
	int err_flag;

	semflg = SEM_R | SEM_A | (SEM_R >> 3) | IPC_CREAT;
	opterr = 0;
	err_flag = 0;

	while ((c = getopt (argc, argv, "e")) != EOF) {
		switch (c) {
			case 'e':
				semflg |= IPC_EXCL;
				break;

			default:
				err_flag++;
				break;
		}
	}

	if (optind != argc - 2)
		err_flag++;

	if (err_flag)
		err_quit ("Usage: semcreat [-e] path num_sems");

	num_sems = atoi (argv [optind + 1]);
	if ((key = ftok (argv [optind], 0x42)) == -1)
		err_msg ("ftok failed");
	if ((semid = semget (key, num_sems, semflg)) == -1)
		err_msg ("semget failed");

	printf ("Semaphore set ID = %d\n", semid);

	return (0);
}
