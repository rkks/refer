#include <unistd.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	daemon_init (argv [0], 0);
	pause ();

	return (0);
}
