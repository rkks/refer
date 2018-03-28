/* checkmail:  watch user's mailbox */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
char *progname;
char *maildir = "/usr/spool/mail";	/* system dependent */

main(argc, argv)
	int argc;
	char *argv[];
{
	struct stat buf;
	char *name, *getlogin();
	int lastsize = 0;

	progname = argv[0];
	if ((name = getlogin()) == NULL)
		error("can't get login name", (char *) 0);
	if (chdir(maildir) == -1)
		error("can't cd to %s", maildir);
	for (;;) {
		if (stat(name, &buf) == -1)	/* no mailbox */
			buf.st_size = 0;
		if (buf.st_size > lastsize)
			fprintf(stderr, "\nYou have mail\007\n");
		lastsize = buf.st_size;
		sleep(60);
	}
}
#include "error.c"
