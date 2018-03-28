/*   Copyright (c) 1988 by George M. Sipe.  All rights reserved.

This software may only be redistributed without fee and without any
other form of monetary gain (including sold, rented, leased, or
traded), unless the express written permission of the copyright holder
is obtained in advance.

This copyright notice must be reproduced in its entirety on all copies
of this software.  Further, acknowledgment of the authorship of this
software must not be removed from its current or derived
documentation.

No expressed or implied warranty is made for this software.  No party
connected with this software assumes any liability or responsibility
for its use, the correctness of its operation, or its fitness for any
purpose.

Any distributor of copies of this software shall grant the recipient
permission for further redistribution as permitted by this notice.

Permission is hereby granted to copy, reproduce, redistribute and
otherwise use this software as long as the conditions above are
strictly adhered to.

	NOTE:  This software was originally written by Dan Heller
	(argv@sri-spam.arpa) and enhanced / generalized by George M.
	Sipe (rebel!george) to the point where Dan would no longer
	recoginze it.  No copyright notices were embodied in the
	original net distribution.

	Steve Alter (alter@ttidca.tti.com) added the -h option.

	Anton Chernoff (abc@lpi.uucp) added the -s option.
*/

#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <sys/types.h>

#ifndef	DELAY
#define	DELAY	15		/* default delay, if not otherwise set */
#endif	DELAY
#ifndef	BASELINE
#define	BASELINE 2		/* default line number for output */
#endif	BASELINE

#define max(a,b) 		(a < b ? b : a)
#define equal(str1,str2)	!strcmp(str1, str2)

#define pinput 			fildes[0]
#define poutput			fildes[1]

#ifdef	BSD
#include <sys/wait.h>
extern char *index();
extern time_t time();
extern int exit();
extern int _exit();
extern int perror();
extern int sleep();
#else
union wait {
	int w_status;
};
#define	vfork()		fork()				/* fork on non-BSD */
#define	dup2(from,to)	close(to); (void) dup(from)	/* good enough here */
#define	index(str,chr)	strchr(str,chr)			/* strchr on non-BSD */
extern char *strchr();
extern long time();
extern void exit();
extern void _exit();
extern void perror();
extern unsigned int sleep();
#endif	BSD

extern int optind;
extern char *optarg;

long wait_delay = DELAY;	/* delay interval */
int force = 0;			/* true to force continued execution */
int needshell = 0;		/* non-zero if subshell needed */

/* create command line string for display */
static char *command(argc, argv)
int argc;
char **argv;
{
	static char string[255];
	int count = 0;
	char *cp = string;

	do {
		(void) sprintf(cp, "%s ", argv[count]);
		cp += strlen(cp);
		needshell |= (int) index(argv[count], '|');
		needshell |= (int) index(argv[count], ';');
	} while (++count < argc);
	*--cp = '\000';
	return (string);
}

/* all done, cleanup and exit */
static int terminate(cause)
{
#ifdef	BSD
	(void) sigsetmask(-1);
#endif
	move(LINES - 1, 0);
	clrtoeol();
	echo();
	refresh();
	endwin();
	exit(cause);
}

int main(argc, argv)
int argc;
char **argv;
{
	register int curline;
	register int iteration = 0;
	register int c;
	int sw;
	int badsw = 0;
	int baseline = BASELINE;
	int doheader = 1;
	int flagchanges = 0;
	int flagch = 0;
	int y, x;
	int somode;
	long nextcycle;
	long curtime;
	long delta;
	int fildes[2];
	int pid;
	union wait status;
	char *cmd = argv[0];
	char *shcmd[4];
	FILE *fp;

	(void) signal(SIGQUIT, terminate);	/* exit when requested */
	(void) signal(SIGINT, terminate);	/* exit when requested */
	(void) signal(SIGPIPE, SIG_IGN);	/* when output > screen size */

	while ((sw = getopt (argc, argv, "d:fhs")) != EOF)
		switch (sw) {
			case 'd':
				wait_delay = atoi(optarg);
				if (wait_delay < 1) wait_delay = 1;
				break;
			case 'f':
				force = !force;
				break;
			case 'h':
				doheader = 0;
				baseline = 0;
				break;
			case 's':
				flagchanges = !flagchanges;
				break;
			case '?':
			default:
				badsw = 1;
				break;
		}
	argv = &argv[optind];
	argc -= optind;
	if (badsw || argc < 1) {
		(void) fprintf(stderr,
			"Usage: %s [-d delay] [-f] [-h] [-s] command [args]\n",
			cmd);
		exit (-1);
	}
	initscr();
	cmd = command(argc, argv);
	if (needshell != 0) {
		shcmd[0] = "sh";
		shcmd[1] = "-c";
		shcmd[2] = cmd;
		shcmd[3] = (char *) 0;
	}
	if (doheader) {
		if (wait_delay != DELAY) {
			if (strlen(cmd) > COLS-39)
				(void) sprintf(cmd, "%.*s...", COLS-42, cmd);
			mvprintw(0, max((COLS-11)/2, strlen(cmd)+15),
				"Delay:  %d", wait_delay);
		} else if (strlen(cmd) > COLS-21)
			(void) sprintf(cmd, "%.*s...", COLS-24, cmd);
		mvprintw(0, 0, "Command:  %s", cmd);
	}
	noecho();			/* don't let typing ruin our painting */
	nextcycle = time(0) + wait_delay;
	while (1) {
		if (doheader)
			mvprintw(0, COLS - 10, "Exec:  %d", ++iteration);
		if (iteration == 1) refresh();
		(void) pipe(fildes);
		/* fork and exec redirecting stdout thru pipe to curses */
		if (!(pid = vfork())) {
			(void) dup2(poutput, 1);
			(void) dup2(poutput, 2);
			(void) close(pinput);
			if (needshell == 0) (void) execvp(*argv, argv);
			else (void) execv("/bin/sh", shcmd);
			perror(*argv);
			_exit(-1);
		}
		if (pid == -1) terminate(-2);
		if (!(fp = fdopen(pinput, "r"))) terminate(-3);
		(void) close(poutput);
		curline = baseline;
		somode = 0;
		move(curline, 0);
		getyx(stdscr, y, x);
		/* read the command's output */
		while ((c = getc(fp)) != EOF && !ferror(fp) && curline < LINES)
		{

			if (c == '\n') {
				if (x < COLS-1) clrtoeol();
				move(++curline, 0);
				getyx(stdscr, y, x);
			}
			else {
				if (x >= COLS-1)
					continue;
				getyx(stdscr, y, x);
				if (flagch)
					if ((c & 0177) == (inch() & 0177)) {
						if (somode) {
							standend();
							somode = 0;
						}
					}
					else {
						if (!somode) {
							standout();
							somode = 1;
						}
					}
				addch(c);
			}
		}
		if (somode) standend();
		flagch = flagchanges;
		if (ferror(fp)) terminate(-4);
		if (baseline > 1 && curline == LINES && c != EOF)
			--baseline;
		/* we've found the end of file, thus, the end of exec */
		(void) fclose(fp);
		clrtobot();
		move(LINES - 1, 0);
		refresh();
		(void) wait(&status);	/* wait for child to terminate */
		/* if child didn't terminate properly, w_status will != 0 */
		if (status.w_status) {
			if (doheader)
				mvprintw(0, COLS - 12, "!");
			if (!force) terminate(status.w_status);
		}
		curtime = time(0);
		delta = nextcycle - curtime;
		if (delta > 0) {
			nextcycle = curtime + delta + wait_delay;
			(void) sleep((unsigned) delta);
		} else nextcycle = curtime + wait_delay;
	}
}
