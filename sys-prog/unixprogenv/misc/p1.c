/* p:  print input in chunks (version 1) */

#include <stdio.h>
#define	PAGESIZE	22
char	*progname;	/* program name for error message */

main(argc, argv)
	int argc;
	char *argv[];
{
	int i;
	FILE *fp, *efopen();

	progname = argv[0];
	if (argc == 1)
		print(stdin, PAGESIZE);
	else
		for (i = 1; i < argc; i++) {
			fp = efopen(argv[i], "r");
			print(fp, PAGESIZE);
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

#include "ttyin1.c"
#include "efopen.c"
