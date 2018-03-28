#include <stdio.h>

int main (void)
{
	int i = 12345;
	float f = 1234.56789123456789;
	char *s = "This string is 34 characters long.";

	printf ("i = '%d'\n", i);
	printf ("f = '%e'\n", f);
	printf ("f = '%f'\n", f);
	printf ("f = '%g'\n", f);
	printf ("s = '%s'\n", s);

	return (0);
}
