#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/sem.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	int c;
	int i;
	int semid;
	key_t key;
	int semflg;
	int num_ops;
	int err_flag;
	struct sembuf *buf;

	semflg = 0;
	opterr = 0;
	err_flag = 0;

	while ((c = getopt (argc, argv, "nu")) != EOF) {
		switch (c) {
			case 'n':
				semflg |= IPC_NOWAIT;
				break;

			case 'u':
				semflg |= SEM_UNDO;
				break;

			default:
				err_flag++;
				break;
		}
	}

	if (argc - optind < 2)
		err_flag++;
	if (err_flag)
		err_quit ("Usage: semops [-nu] path operation ...");

	if ((key = ftok (argv [optind], 0x42)) == -1)
		err_msg ("ftok failed");
	if ((semid = semget (key, 0, 0)) == -1)
		err_msg ("semget failed");
	optind++;
	num_ops = argc - optind;

	if ((buf = calloc (num_ops, sizeof (struct sembuf))) == NULL)
		err_msg ("calloc failed");

	for (i = 0; i < num_ops; i++) {
		buf [i].sem_num = i;
		buf [i].sem_op = atoi (argv [optind + i]);
		buf [i].sem_flg = semflg;
	}

	if (semop (semid, buf, num_ops) == -1)
		err_msg ("semop failed");

	return (0);
}
