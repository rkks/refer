#include <stdio.h>

struct a {
	unsigned int x:19;
};

int main (void)
{
	struct a foo;
	unsigned long addr;

	foo.x = 0x40000;

	addr = foo.x << 13;
	printf ("addr = 0x%lx\n", addr);

	addr = (unsigned int) (foo.x << 13);
	printf ("addr = 0x%lx\n", addr);

	return (0);
}
