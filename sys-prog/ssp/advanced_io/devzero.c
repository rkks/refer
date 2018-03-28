#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "ssp.h"

#define NUM_ITERS 1000

int main (void)
{
	int i;
	int fd;
	int counter;
	pid_t pid;
	int *ptr;

	if ((fd = open ("/dev/zero", O_RDWR)) == -1)
		err_msg ("open failed");
	if ((ptr = mmap (0, sizeof (int), PROT_READ | PROT_WRITE, MAP_SHARED,
		fd, 0)) == MAP_FAILED) {
		err_msg ("mmap failed");
	}
	close (fd);

	if (ssp_tell_wait_init () == -1)
		err_msg ("ssp_tell_wait_init failed");

	switch (pid = fork ()) {
		case -1:
			err_msg ("Can't fork");

		case 0:
			for (i = 1; i < NUM_ITERS + 1; i += 2) {
				if (ssp_wait_parent () == -1)
					err_msg ("ssp_wait_parent failed");
				if ((counter = (*ptr)++) != i)
					err_quit ("Child: Got %d, expected %d", counter, i);
				ssp_tell_parent (getppid ());
			}
			_exit (0);

		default:
			for (i = 0; i < NUM_ITERS; i += 2) {
				if ((counter = ((*ptr)++)) != i)
					err_quit ("Parent: Got %d, expected %d", counter, i);
				if (ssp_tell_child (pid) == -1)
					err_msg ("ssp_tell_child failed");
				if (ssp_wait_child () == -1)
					err_msg ("ssp_wait_child failed");
			}
			break;
	}

	return (0);
}
