#include <stdio.h>

int main (void)
{
	int i = 12345;
	float f = 1234.56789123456789;
	char *s = "This string is 34 characters long.";

	printf ("i = '%09d'\n", i);
	printf ("f = '%.9e'\n", f);
	printf ("f = '%+.9f'\n", f);
	printf ("f = '%.9g'\n", f);
	printf ("s = '%-30.20s'\n", s);

	return (0);
}
