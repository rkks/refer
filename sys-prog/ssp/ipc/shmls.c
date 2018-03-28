#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include "ssp.h"

static void print_shm_seg (int shmid);

int main (int argc, char **argv)
{
	int c;
	int err_flag;
	int shmid;
	int *buf;
	uint_t i;
	uint_t n;
	uint_t nids;

	nids = 0;
	buf = NULL;
	shmid = 0;
	opterr = 0;
	err_flag = 0;

	while ((c = getopt (argc, argv, "m:")) != EOF) {
		switch (c) {
			case 'm':
				shmid = atoi (optarg);
				break;

			default:
				err_flag++;
				break;
		}
	}

	if (optind != argc)
		err_flag++;
	if (err_flag)
		err_quit ("Usage: shmls [-m shared memory segment ID]");

	if (shmid != 0)
		print_shm_seg (shmid);
	else {
		for (;;) {
			if (shmids (buf, nids, &n) == -1)
				err_msg ("shmids failed");

			if (n <= nids)
				break;

			nids = n;
			if ((buf = realloc (buf, nids * sizeof (int))) == NULL)
				err_msg ("realloc failed");
		}

		for (i = 0; i < n; i++)
			print_shm_seg (buf [i]);

		free (buf);
	}

	return (0);
}

static void print_shm_seg (int shmid)
{
	struct shmid_ds shm_info;

	if (shmctl (shmid, IPC_STAT, &shm_info) == -1)
		err_msg ("shmctl failed");

	printf ("Shared memory segment ID %d:\n", shmid);
	printf ("  size = %d bytes\n", shm_info.shm_segsz);
}
