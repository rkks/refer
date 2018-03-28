#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <door.h>
#include "ssp.h"

static void server_proc (void *cookie, char *argp, size_t arg_size,
	door_desc_t *dp, uint_t n_desc);

int main (int argc, char **argv)
{
	int door_fd;
	int tmp_fd;

	if (argc != 2)
		err_quit ("Usage: desc_door_srv path");

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
	int fd;
	char buf [BUF_SIZE];
	door_desc_t desc;

	argp [arg_size - 1] = '\0';
	if ((fd = open (argp, O_RDONLY)) == -1) {
		snprintf (buf, BUF_SIZE, "%s: %s", argp, strerror (errno));
		if (door_return (buf, strlen (buf), NULL, 0) == -1)
			err_msg ("door_return failed");
	}

	printf ("Returning descriptor %d\n", fd);

	desc.d_attributes = DOOR_DESCRIPTOR;
	desc.d_data.d_desc.d_descriptor = fd;

	if (door_return (NULL, 0, &desc, 1) == -1)
		err_msg ("door_return failed");
}
