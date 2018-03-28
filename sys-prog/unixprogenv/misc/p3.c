/* p:  print input in chunks (version 3) */

#include <stdio.h>
#define	PAGESIZE	22
char	*progname;	/* program name for error message */

main(argc, argv)
	int argc;
	char *argv[];
{
	FILE *fp, *efopen();
	int i, pagesize = PAGESIZE;
	char *p, *getenv();

	progname = argv[0];
	if ((p=getenv("PAGESIZE")) != NULL)
		pagesize = atoi(p);
	if (argc > 1 && argv[1][0] == '-') {
		pagesize = atoi(&argv[1][1]);
		argc--;
		argv++;
	}
	if (argc == 1)
		print(stdin, pagesize);
	else
		for (i = 1; i < argc; i++) {
			fp = efopen(argv[i], "r");
			print(fp, pagesize);
			fclose(fp);
		}
	exit(0);
}

print(fp, pagesize)	/* print fp in pagesize chunks */
	FILE *fp;
	int pagesize;
{
	static int lines = 0;	/* number of lines so far */
	char buf[BUFSIZ];

	while (fgets(buf, sizeof buf, fp) != NULL)
		if (++lines < pagesize)
			fputs(buf, stdout);
		else {
			buf[strlen(buf)-1] = '\0';
			fputs(buf, stdout);
			fflush(stdout);
			ttyin();
			lines = 0;
		}
}

#include "ttyin2.c"
#include "efopen.c"
