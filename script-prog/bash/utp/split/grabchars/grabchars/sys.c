/*
**	$Header: sys.c,v 1.9 89/12/29 21:14:55 daniel grabchars_1_9 $
**
**	sys.c - terminal routines for grabchars
**
**	Dan Smith (daniel@island.uu.net), November 29, 1988
**
**	History:
**
**	December 2, 1988
**	made #ifdefs for DV_ERASE and BSD, wrote notes in handle_erase ()
**	for changes and improvements
**
**	May 11, 1989
**	Started to add SYS_V code
**
**	December 26, 1989
**	added flush/no flush for System V
*/

#include <stdio.h>
#include <signal.h>
#include "grabchars.h"


#ifdef BSD
#	include <sgtty.h>
	struct sgttyb orig, new;
#endif /* BSD */

#ifdef SYS_V
#	include <termio.h>
	struct termio orig, new;
#endif /* SYS_V */


/* all declared in globals.c */
extern FILE *outfile, *otherout;
extern FLAG *flags;
extern int exit_stat;
extern char default_string[128];

#ifdef DV_ERASE
extern char *erase_buf;
#endif

/* initialize global flags */
init_flags ()
{
	flags->both = 0;
	flags->check = 0;
	flags->dflt = 0;
	flags->flush = 0;
	flags->ret_key = 0;
	flags->silent = 0;
	flags->erase = 0;
	flags->lower = 0;
	flags->upper = 0;
}

/*
**	initialize tty
*/
init_term ()
{
	/*	play havoc with the terminal :-) */

#ifdef BSD
	ioctl (0, TIOCGETP, &orig);
	new = orig;
	new.sg_flags &= ~ECHO;
	new.sg_flags |= CBREAK;

	(flags->flush) ? ioctl (0, TIOCSETP, &new) :	/* to flush... */
		ioctl (0, TIOCSETN, &new);		/* ...or not to flush */
#endif

#ifdef SYS_V
	ioctl(0, TCGETA, &orig);           /* Added 6/30/89 for Sys V - rjd */
	new = orig;                      /* Added 6/30/89 for Sys V - rjd */
	new.c_iflag = 0;                   /* Added 6/30/89 for Sys V - rjd */
	new.c_iflag |= ICRNL | ISTRIP;     /* Added 6/30/89 for Sys V - rjd */
	new.c_lflag &= ~(ISIG | ICANON | ECHO);  /* Added 6/30/89 for Sys V - rjd */
	new.c_cc[4] = new.c_cc[5] = '\001';  /* Added 6/30/89 for Sys V - rjd */

	/* 12/26/89....flush for SYS V...daniel */
	(flags->flush) ? ioctl(0, TCSETAF, &new) :
		ioctl(0, TCSETA, &new);

#endif /* SYS_V */

}

/* handle the outside world */
init_signal ()
{
	int lets_go ();

#ifdef BSD
	(SIGRET) signal (SIGTSTP, lets_go);
#endif
	(SIGRET) signal (SIGINT, lets_go);
	(SIGRET) signal (SIGQUIT, lets_go);
}

/*
**	something's up with the user...give a useful exit status so
**	we can ask things like "do you need help?"
*/
int overtime ()
{
	int lets_go ();
	void handle_default ();

	/* does not return */
	if (exit_stat == -1 && flags->dflt)
		handle_default ();

	exit_stat = -2;
	lets_go ();
}

/*
**	the default_flag is set, and the user either typed a return
**	or timed out.  This routine does not return.
*/
void handle_default ()
{
	int lets_go ();

	if (! flags->silent) {
		fputs (default_string, outfile);
		if (flags->both || flags->ret_key)
			fputs (default_string, otherout);
	}
	exit_stat = strlen (default_string);
	lets_go ();
}

/*	clean up and get out of here... */
int lets_go ()
{
#ifdef BSD
	ioctl (0, TIOCSETP, &orig);
#endif

#ifdef SYS_V
	ioctl(0, TCSETA, &orig);        /* Added 6/30/89 for Sys V - rjd */
#endif
	exit (exit_stat);
}

#ifdef DV_ERASE

/*
**	December 2, 1988
**	in progress notes for changing this...
**
**	first time through processing should be called as its' own
**	function from the -E case in the main (getopt ()) switch...
**
**	stdout and stderr should never be affected by any erasures...
**	(they probably are not now, I haven't thoroughly tested this...)
**
**	I need to drag in my word erase routine; never can tell how
**	long some people are going to want their lines with -n! :-)
**
**	If someone wants a control char (via literal (^V)), we should
**	give it to them...this would be more compatible with $< (csh)
**	and read (sh)... grabchars almost completely replaces these
**	now.
**
**	we can also be sensitive to pipe/no pipe via isatty ()...
*/

/*	DV: handle erase characters, kill characters, etc. */
handle_erase (ch, cnt)
char ch;
int *cnt;
{
	static char first = 1;
	static char erasec, killc, werasec, lnextc, rprntc;
	static char lnextflg = 0;
	static char *cp;
	static FILE *tty;
	int i;

	if (first) {
		/* initialize static things */
		struct sgttyb sb;
		struct ltchars ltc;

		first = 0;
		cp = erase_buf;
		tty = fopen ("/dev/tty", "w");

		/* this isn't going to do... what if -e is set?...dan */
		if (tty == NULL)
			tty = stderr;
		ioctl (0, TIOCGETP, &sb);
		ioctl (0, TIOCGLTC, &ltc);
		erasec = sb.sg_erase;
		killc = sb.sg_kill;
		werasec = ltc.t_werasc;
		lnextc = ltc.t_lnextc;
		rprntc = ltc.t_rprntc;
	}

	if (lnextflg) {
		ch |= 0x80;
		lnextflg = 0;
	}
	(*cnt) --;
	if (ch == erasec) {
		if (*cnt < 0)
			return;
		fprintf (tty, "\b \b");
		(*cnt) --;
		*--cp = 0;
	} else if (ch == killc) {
		while (*cnt >= 0) {
			fprintf (tty, "\b \b");
			(*cnt) --;
			*--cp = 0;
		}
	} else if (ch == werasec) {
		if (*cnt < 0)
			return;
		while ((cp[-1] == ' ' || cp[-1] == '\t') && (*cnt) >= 0) {
			fprintf (tty, "\b");
			(*cnt) --;
			*--cp = 0;
		}
		while (cp[-1] != ' ' && cp[-1] != '\t' && (*cnt) >= 0) {
			fprintf (tty, "\b \b");
			(*cnt) --;
			*--cp = 0;
		}
	} else if (ch == lnextc) {
		lnextflg = 1;
		fprintf (tty, "^\b");
	} else if (ch == rprntc) {
		for (i = strlen (erase_buf); i > 0; i--)
			putc ('\b', tty);
		fprintf (tty, "%s", erase_buf);
	} else {
		ch &= 0x7f;
		fprintf (tty, "%c", ch);
		*cp++ = ch;
		(*cnt) ++;
	}
	fflush (tty);
	return;
}
#endif
