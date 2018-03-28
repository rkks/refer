#include <stdio.h>
#include <unistd.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	long filesize_bits;
	long max_links;

	if (argc != 2)
		err_quit ("Usage: pathconf path");

	filesize_bits = pathconf (argv [1], _PC_FILESIZEBITS);
	max_links = pathconf (argv [1], _PC_LINK_MAX);

	printf ("Maximum file size bits = %ld\n", filesize_bits);
	printf ("Maximum links to a file = %ld\n", max_links);

	return (0);
}
