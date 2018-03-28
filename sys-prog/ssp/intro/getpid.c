#include <stdio.h>
#include <unistd.h>

int main (void)
{
	printf ("PID = %ld, PPID = %ld\n", (long) getpid (), (long) getppid ());

	return (0);
}
