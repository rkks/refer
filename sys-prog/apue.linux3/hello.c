#include	"ourhdr.h"

int
main(void)
{
	printf("hello world from process ID %d\n", getpid());
	exit(0);
}
