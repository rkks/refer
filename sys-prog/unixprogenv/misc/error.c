error(s1, s2)	/* print error message and die */
	char *s1, *s2;
{
	extern int errno, sys_nerr;
	extern char *sys_errlist[], *progname;

	if (progname)
		fprintf(stderr, "%s: ", progname);
	fprintf(stderr, s1, s2);
	if (errno > 0 && errno < sys_nerr)
		fprintf(stderr, " (%s)", sys_errlist[errno]);
	fprintf(stderr, "\n");
	exit(1);
}
