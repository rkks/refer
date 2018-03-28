#include <stdio.h>
#include <stdlib.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	int status;

	status = system (argv [1]);
	print_term_status (status);

	return (0);
}
