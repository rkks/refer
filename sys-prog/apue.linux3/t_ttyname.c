#include	"ourhdr.h"

int
main(void)
{
	printf("fd 0: %s\n", isatty(0) ? ttyname(0) : "not a tty");
	printf("fd 1: %s\n", isatty(1) ? ttyname(1) : "not a tty");
	printf("fd 2: %s\n", isatty(2) ? ttyname(2) : "not a tty");
	exit(0);
}
