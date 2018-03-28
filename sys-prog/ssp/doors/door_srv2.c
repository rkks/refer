#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <door.h>
#include "ssp.h"

static void server_proc (void *cookie, char *argp, size_t arg_size,
	door_desc_t *dp, uint_t n_desc);

int main (int argc, char **argv)
{
	int door_fd;
	int tmp_fd;

	if (argc != 2)
		err_quit ("Usage: door_srv2 path");

	if ((door_fd = door_create (server_proc, NULL, 0)) == -1)
		err_msg ("door_create failed");

	unlink (argv [1]);
	if ((tmp_fd = creat (argv [1], FILE_PERMS)) == -1)
		err_msg ("creat failed");
	close (tmp_fd);
	if (fattach (door_fd, argv [1]) == -1)
		err_msg ("fattach failed");

	for (;;)
		pause ();
}

static void server_proc (void *cookie, char *argp, size_t arg_size,
	door_desc_t *dp, uint_t n_desc)
{
	long arg;
	long res;
	door_cred_t info;

	if (door_cred (&info) == -1)
		err_msg ("door_cred failed");
	printf ("Client credentials:\n");
	printf ("  Effective user ID = %ld\n", info.dc_euid);
	printf ("  Effective group ID = %ld\n", info.dc_egid);
	printf ("  Real user ID = %ld\n", info.dc_ruid);
	printf ("  Real group ID = %ld\n", info.dc_rgid);
	printf ("  Process ID = %ld\n", info.dc_pid);

	arg = *((long *) argp);
	res = arg * arg;

	if (door_return ((char *) &res, sizeof (long), NULL, 0) == -1)
		err_msg ("door_return failed");
}
