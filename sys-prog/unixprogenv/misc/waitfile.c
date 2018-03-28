/* waitfile:  wait until file stops changing */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
char *progname;

main(argc, argv)
	int argc;
	char *argv[];
{
	int fd;
	struct stat stbuf;
	time_t old_time = 0;

	progname = argv[0];
	if (argc < 2)
		error("Usage: %s filename [cmd]", progname);
	if ((fd = open(argv[1], 0)) == -1)
		error("can't open %s", argv[1]);
	fstat(fd, &stbuf);
	while (stbuf.st_mtime != old_time) {
		old_time = stbuf.st_mtime;
		sleep(60);
		fstat(fd, &stbuf);
	}
	if (argc == 2) {	/* copy file */
		execlp("cat", "cat", argv[1], (char *) 0);
		error("can't execute cat %s", argv[1]);
	} else {			/* run process */
		execvp(argv[2], &argv[2]);
		error("can't execute %s", argv[2]);
	}
	exit(0);
}

#include "error.c"
