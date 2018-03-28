#include <stdio.h>
#include <string.h>
#include <sys/types.h>

int main (void)
{
	fd_set read_set;
	fd_set write_set;
	int i;

	FD_ZERO (&read_set);
	FD_ZERO (&write_set);

	FD_SET (0, &read_set);
	FD_SET (1, &write_set);
	FD_SET (2, &read_set);
	FD_SET (3, &write_set);

	printf ("read_set:\n");
	for (i = 0; i < 4; i++) {
		printf ("  bit %d is %s\n", i,
			(FD_ISSET (i, &read_set)) ? "set" : "clear");
	}

	printf ("write_set:\n");
	for (i = 0; i < 4; i++) {
		printf ("  bit %d is %s\n", i,
			(FD_ISSET (i, &write_set)) ? "set" : "clear");
	}

	return (0);
}
