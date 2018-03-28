#include <unistd.h>
#include <stdlib.h>

static void ssp_closefrom (int lowfd);
static int close_func (void *lowfd, int fd);

int main (void)
{
	ssp_closefrom (0);

	return (0);
}

static void ssp_closefrom (int lowfd)
{
	fdwalk (close_func, &lowfd);
}

static int close_func (void *lowfd, int fd)
{
	if (fd >= *(int *)lowfd)
		close (fd);

	return (0);
}
