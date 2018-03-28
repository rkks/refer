#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "ssp.h"

static void queue_snapshot (int msqid, long msgtyp);

int main (int argc, char **argv)
{
	int c;
	int err_flag;
	long msgtyp;
	int msqid;
	int *buf;
	uint_t i;
	uint_t n;
	uint_t nids;

	nids = 0;
	buf = NULL;
	msgtyp = 0;
	msqid = 0;
	opterr = 0;
	err_flag = 0;

	while ((c = getopt (argc, argv, "t:q:")) != EOF) {
		switch (c) {
			case 't':
				msgtyp = atol (optarg);
				break;

			case 'q':
				msqid = atoi (optarg);
				break;

			default:
				err_flag++;
				break;
		}
	}

	if (optind != argc)
		err_flag++;
	if (err_flag)
		err_quit ("Usage: msgls [-t type] [-q queue ID]");

	if (msqid != 0)
		queue_snapshot (msqid, msgtyp);
	else {
		for (;;) {
			if (msgids (buf, nids, &n) == -1)
				err_msg ("msgids failed");

			if (n <= nids)
				break;

			nids = n;
			if ((buf = realloc (buf, nids * sizeof (int))) == NULL)
				err_msg ("realloc failed");
		}

		for (i = 0; i < n; i++)
			queue_snapshot (buf [i], msgtyp);

		free (buf);
	}

	return (0);
}

static void queue_snapshot (int msqid, long msgtyp)
{
	size_t buf_size;
	struct msgsnap_head *buf;
	struct msgsnap_mhead *mhead;
	size_t i;
	size_t mlen;

	buf_size = sizeof (struct msgsnap_head);
	if ((buf = malloc (buf_size)) == NULL)
		err_msg ("malloc failed");

	for (;;) {
		if (msgsnap (msqid, buf, buf_size, msgtyp) == -1)
			err_msg ("msgsnap failed");

		if (buf -> msgsnap_size <= buf_size)
			break;

		buf_size = buf -> msgsnap_size;
		if ((buf = realloc (buf, buf_size)) == NULL)
			err_msg ("realloc failed");
	}

	printf ("Queue ID %d:\n", msqid);
	mhead = (struct msgsnap_mhead *) (buf + 1);
	for (i = 0; i < buf -> msgsnap_nmsg; i++) {
		mlen = mhead -> msgsnap_mlen;
		msgtyp = mhead -> msgsnap_mtype;

		printf ("  %d %d bytes, message type = %ld\n", i, mlen, msgtyp);

		mhead = (struct msgsnap_mhead *)
			((char *) mhead + sizeof (struct msgsnap_mhead) +
			((mlen + sizeof (size_t) - 1) & ~(sizeof (size_t) - 1)));
	}

	free (buf);
}
