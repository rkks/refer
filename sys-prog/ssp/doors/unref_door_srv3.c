#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <door.h>
#include "ssp.h"

static int door_fd;
static char *door_path;

static void server_proc (void *cookie, char *argp, size_t arg_size,
	door_desc_t *dp, uint_t n_desc);

int main (int argc, char **argv)
{
	int tmp_fd;

	if (argc != 2)
		err_quit ("Usage: unref_door_srv3 path");

	if ((door_fd = door_create (server_proc, NULL, DOOR_UNREF)) == -1)
		err_msg ("door_create failed");

	door_path = argv [1];
	unlink (door_path);
	if ((tmp_fd = creat (door_path, FILE_PERMS)) == -1)
		err_msg ("creat failed");
	close (tmp_fd);
	if (fattach (door_fd, door_path) == -1)
		err_msg ("fattach failed");

	for (;;)
		pause ();
}

static void server_proc (void *cookie, char *argp, size_t arg_size,
	door_desc_t *dp, uint_t n_desc)
{
	long arg;
	long res;

	if (argp == DOOR_UNREF_DATA) {
		printf ("Door unreferenced\n");
		if (fattach (door_fd, door_path) == -1)
			 err_msg ("fattach failed");
		if (door_return (NULL, 0, NULL, 0) == -1)
			err_msg ("door_return failed");
	}

	if (fdetach (door_path) == -1)
		err_msg("fdetach failed");

	arg = *((long *) argp);
	res = arg * arg;

	printf ("Server proc returning, res = %ld\n", res);

	if (door_return ((char *) &res, sizeof (long), NULL, 0) == -1)
		err_msg ("door_return failed");
}
