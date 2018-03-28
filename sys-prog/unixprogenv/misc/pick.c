/* pick:  offer choice on each argument */

#include <stdio.h>
char	*progname;	/* program name for error message */

main(argc, argv)
	int argc;
	char *argv[];
{
	int i;
	char buf[BUFSIZ];

	progname = argv[0];
	if (argc == 2 && strcmp(argv[1],"-") == 0)	/* pick - */
		while (fgets(buf, sizeof buf, stdin) != NULL) {
			buf[strlen(buf)-1] = '\0';  /* drop newline */
			pick(buf);
		}
	else
		for (i = 1; i < argc; i++)
			pick(argv[i]);
	exit(0);
}

pick(s)	/* offer choice of s */
	char *s;
{
	fprintf(stderr, "%s? ", s);
	if (ttyin() == 'y')
		printf("%s\n", s);
}

#include "ttyin2.c"
#include "efopen.c"
