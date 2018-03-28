/* cp:  minimal version */
#include <stdio.h>
#define PERMS 0644 /* RW for owner, R for group, others */
char *progname;

main(argc, argv)	/* cp: copy f1 to f2 */
	int argc;
	char *argv[];
{
	int f1, f2, n;
	char buf[BUFSIZ];

	progname = argv[0];
	if (argc != 3)
		error("Usage: %s from to", progname);
	if ((f1 = open(argv[1], 0)) == -1)
		error("can't open %s", argv[1]);
	if ((f2 = creat(argv[2], PERMS)) == -1)
		error("can't create %s", argv[2]);

	while ((n = read(f1, buf, BUFSIZ)) > 0)
		if (write(f2, buf, n) != n)
			error("write error", (char *) 0);
	exit(0);
}

#include "error.c"
