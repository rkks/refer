#include	<signal.h>
#include	<stdio.h>
#include	<termios.h>

#define	MAX_PASS_LEN	8		/* max #chars for user to enter */

char *
getpass(const char *prompt)
{
	static char		buf[MAX_PASS_LEN + 1];	/* null byte at end */
	char			*ptr;
	sigset_t		sig, sigsave;
	struct termios	term, termsave;
	FILE			*fp;
	int				c;

	if ( (fp = fopen(ctermid(NULL), "r+")) == NULL)
		return(NULL);
	setbuf(fp, NULL);

	sigemptyset(&sig);	/* block SIGINT & SIGTSTP, save signal mask */
	sigaddset(&sig, SIGINT);
	sigaddset(&sig, SIGTSTP);
	sigprocmask(SIG_BLOCK, &sig, &sigsave);

	tcgetattr(fileno(fp), &termsave);	/* save tty state */
	term = termsave;					/* structure copy */
	term.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL);
	tcsetattr(fileno(fp), TCSAFLUSH, &term);

	fputs(prompt, fp);

	ptr = buf;
	while ( (c = getc(fp)) != EOF && c != '\n') {
		if (ptr < &buf[MAX_PASS_LEN])
			*ptr++ = c;
	}
	*ptr = 0;			/* null terminate */
	putc('\n', fp);		/* we echo a newline */

						/* restore tty state */
	tcsetattr(fileno(fp), TCSAFLUSH, &termsave);

						/* restore signal mask */
	sigprocmask(SIG_SETMASK, &sigsave, NULL);
	fclose(fp);			/* done with /dev/tty */

	return(buf);
}
