#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <rctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ssp.h"

extern void print_rctl (rctlblk_t *rblk);

int main (void)
{
	rctlblk_t *old_rblk;
	rctlblk_t *new_rblk;
	char *name = "process.max-file-descriptor";
	int fd;

	if (((old_rblk = malloc (rctlblk_size ())) == NULL) ||
		((new_rblk = malloc (rctlblk_size ())) == NULL))
		err_msg ("malloc failed");

	if (getrctl (name, NULL, old_rblk, RCTL_FIRST) == -1)
		err_msg ("getrctl failed");

	memcpy (new_rblk, old_rblk, rctlblk_size ());

	printf ("Before...\n");
	print_rctl (old_rblk);

	rctlblk_set_value (new_rblk, 10);
	rctlblk_set_local_action (new_rblk, RCTL_LOCAL_DENY | RCTL_LOCAL_SIGNAL,
		SIGTERM);

	if (setrctl (name, NULL, old_rblk, RCTL_DELETE) == -1)
		err_msg ("setrctl (RCTL_DELETE) failed");
	if (setrctl (name, NULL, new_rblk, RCTL_INSERT) == -1)
		err_msg ("setrctl (RCTL_INSERT) failed");

	if (getrctl (name, NULL, new_rblk, RCTL_FIRST) == -1)
		err_msg ("getrctl failed");

	printf ("After...\n");
	print_rctl (new_rblk);

	for (;;) {
		fd = open ("/tmp", O_RDONLY);
		printf ("Returned file descriptor = %d\n", fd);
	}
}
