#include <stdio.h>
#include <time.h>

int main (void)
{
	struct tm *tp;
	time_t now;
	int i;
	char buf [BUFSIZ];
	char *formats [] = {
		"%A, %B %e, %Y, %H:%M:%S",
		"%I:%M %p, %d-%b-%y",
		"%x %X",
		"%C",
		"%c",
		NULL
	};

	now = time (NULL);
	tp = localtime (&now);

	for (i = 0; formats [i] != NULL; i++) {
		strftime (buf, BUFSIZ, formats [i], tp);
		printf ("%-30s %s\n", formats [i], buf);
	}

	return (0);
}
