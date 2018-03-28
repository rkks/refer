#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <values.h>

int main (void)
{
	char *ptr;

	ptr = malloc (MAXLONG);

	assert (ptr != NULL);

	printf ("Allocated %ld bytes of memory\n", MAXLONG);

	return (0);
}
