#include <stdio.h>

int main (void)
{
	char *one = "one";
	int two = 2;
	char *three = "three";

	printf ("%1$s %2$d %3$s %1$s\n", one, two, three);

	return (0);
}
