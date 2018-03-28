#include	"call.h"

/* Copy a file from local to remote. */

void
put(int remfd)
{
	int		i, n, linecnt;
	char	c, cmd[MAXLINE];
	FILE	*fpin;

	if (prompt_read("~[put] ", take_put_args) < 0) {
		printf("usage: [put] <sourcefile> <destfile>\n\r");
		return;
	}

			/* open local input file */
	if ( (fpin = fopen(src, "r")) == NULL) {
		err_ret("can't open %s for reading", src);
		putc('\r', stderr);
		fflush(stderr);
		return;
	}

			/* send stty/cat/stty command to remote host */
	sprintf(cmd, "stty -echo; cat >%s; stty echo\r", dst);
	n = strlen(cmd);
	if (write(remfd, cmd, n) != n)
		err_sys("write error");
	tcdrain(remfd);		/* wait for our output to be sent */
	sleep(4);			/* and let stty take effect */

			/* send file to remote host */
	linecnt = 0;
	for ( ; ; ) {
		if ( (i = getc(fpin)) == EOF)
			break;			/* all done */
		c = i;
		if (write(remfd, &c, 1) != 1)
			break;			/* line has probably dropped */
		if (c == '\n')		/* but newlines are written to file */
			printf("\r%d", ++linecnt);
	}

			/* send EOF to remote, to terminate cat */
	c = tty_termios()->c_cc[VEOF];
	write(remfd, &c, 1);
	tcdrain(remfd);				/* wait for our output to be sent */
	sleep(2);
	tcflush(remfd, TCIOFLUSH);	/* flush echo of stty/cat/stty */
	c = '\n';
	write(remfd, &c, 1);

	if (ferror(fpin)) {
		err_msg("read error of local file");
		putc('\r', stderr);
		fflush(stderr);
	}
	fclose(fpin);
}
