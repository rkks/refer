#include <stdio.h>

int main (int argc, char **argv)
{
	int i;
	char **p;
	extern char **environ;

	printf ("Command line arguments:\n");
	for (i = 0; i < argc; i++)
		printf ("  argv [%d]: %s\n", i, argv [i]);

	printf ("Environment variables:\n");
	for (p = environ; *p != 0; p++)
		printf ("  %s\n", *p);

	return (0);
}
