#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main (void)
{
	printf ("User ID = %ld, group ID = %ld\n", (long) getuid (),
		(long) getgid ());

	return (0);
}
