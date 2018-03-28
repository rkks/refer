/*
**	$Header: grabchars.c,v 1.9 89/12/29 21:14:48 daniel grabchars_1_9 $
**
**	grabchars.c	- get characters directly from the user
**
**	Dan Smith (daniel@island.uu.net), October 23, 1988
**
**	This program grabs characters from the user as they are
**	typed in, without having to wait for the return key to
**	be pressed.  Among other things, this allows shell scripts
**	to be written with highly interactive menus...
**
**	[to jump right to the code, search for "start of grabchars"]
**
**	Usage rundown:
**
**	grabchars			gets one keystroke
**	grabchars -b			output to stdout and stderr
**	grabchars -c<valid characters>  only <valid chars> are returned
**	grabchars -d<char(s)>		default char or string to return
**	grabchars -e			output to stderr instead of stdout
**	grabchars -f			flush any previous input
**	grabchars -h			help screen
**	grabchars -n<number>		number of characters to read
**	grabchars -p<prompt>		prompt to help user
**	grabchars -q<prompt>		prompt to help user (through stderr)
**	grabchars -r			RETURN key exits (use with -n)
**	grabchars -s			silent, just return status
**	grabchars -t<seconds>		timeout after <seconds>
**	grabchars -E			erase/kill character processing
**	grabchars -L			lower case mapping of chars
**	grabchars -U			upper case mapping of chars
**
**	examples: (values to arguments can be in the same word or the next one)
**
**	grabchars -caeiou	 or
**	grabchars -c aeiou		get one of the vowels
**	grabchars -c i			get the letter 'i'
**	grabchars '-penter a letter '	print the prompt "enter a letter "
**	grabchars '-qenter a letter '	print the prompt ('q' for question)
**					"enter a letter " through stderr...
**	grabchars -n4			get four characters
**	grabchars -t2			timeout after two seconds
**
**	print a prompt and grab three characters...
**	grabchars -p 'enter three characters >> ' -n 3
**
**	get two numbers with a ten second timeout...
**	grabchars -c 0123456789 -n2 -t10
**
**	map 5 characters to upper case, or default to "HELP"
**	grabchars -d "HELP" -n5 -U
**
**	note that arguments like "-n4" or "-n 4" are handled the same way
**
**	History:
**	
**	Oct 1988: versions 1.0 - 1.1
**
**	November 6, 1988 (1.15)
**	added -f flag to flush input, default is to use	TIOCSETN instead
**	of TIOCSETP
**
**	November 22, 1988 (1.16)
**	added -d flag for a default character or string to use if the
**	user hits return first thing or times out.  handle_default ()
**	was added at this time
**
**	November 23, 1988 (1.19)
**	added -r flag to exit when RETURN is hit.  This was suggested by
**	David Vezie.
**
**	November 29, 1988 (1.2)
**	Disaster strikes...I was updating Makefile.dist, and copied SRCS
**	to OBJS, and forgot to change grabchars.c to grabchars.o, and
**	then (after Config) did a "make clean"!  I realized that I did not
**	have a backup, but fortunately had David Vezie's hack of grabchars.c
**	from a few days ago (he had added erase/line kill character processing)
**	...moral: use RCS, check your Makefiles!  Used this as an opportunity
**	to split things up into grabchars.h, globals.c,	and sys.c
**	Got -c to handle ranges (via re_comp() and re_exec())
**
**	December 12, 1988 (1.3.1)
**	This finally appeared on the net.  Grabbed it and added -U and
**	-L to do upper/lower case mapping
**
**	August 17, 1989 (1.5)
**	Finally integrated Randy's changes in for Sys V.
*/

/*
 * Changes made to allow compilation and operation under Unix System V
 * Release 2 or 3 on 6/30/89 by R. J. Davis, ocrjd.att.com!randy
 */

/*	start of grabchars... */
#include <stdio.h>
#include <signal.h>
#include <ctype.h>
#include "grabchars.h"

/*	see globals.c */
extern FILE *outfile, *otherout;
extern FLAG *flags;
extern int exit_stat;
extern char *usage_statement[];

/*
**	David Vezie (unicom!dv) took a great shot at putting in
**	erase/kill processing.  I need to test this some more, and I'll
**	most likely change it a bit.  I put DV_ERASE in the Makefile
**	as a default; take it out if it misbehaves :-)
*/
#ifdef DV_ERASE
extern char *erase_buf;	/* DV: Malloc'ed later */
#endif

main (argc, argv)
int argc;
register char **argv;
{
	/* two signal/wrapup handling routines in sys.c */
	int lets_go (), overtime ();

	/* for -d option */
	void handle_default ();

	/* for getopt () */
	extern int optind, opterr;
	extern char *optarg;
	char comarg;

	int how_many = 1;	/* how many chars to read */
	int num_read;		/* how many we have read... */
	int timeout;		/* and an optional time to do it in... */
	int i;			/* for usage_statement if we need it.. */

	/*
	**	re_comp (), re_exec () let us do things
	**	like "grabchars -c '[a-d]'" or "grabchars -c '[^a-z]'"...
	*/
	char *re_comp (), *re_error;
	extern char valid_chars[128], default_string[128];
	char ch, check_str[2];

	alarm (0);
	opterr = 0;
	exit_stat = -1;	/* if we're interrupted, exit with this status */

	outfile = stdout;
	otherout = stderr;
	if ((flags = (FLAG *) malloc (sizeof (FLAG))) == (FLAG *) NULL) {
		fprintf (stderr, "can't find enough memory...bye!\n");
		exit (exit_stat);	/* we don't need lets_go () for this */
	}

	init_flags ();
	init_signal ();

	while ((comarg = getopt (argc, argv,  "befrsELUc:d:n:p:q:t:")) != EOF) {
		switch (comarg) {
			case 'b':
				flags->both = 1;
				break;
			case 'c':
				flags->check = 1;
				strcpy (valid_chars, optarg);
				if (strlen (optarg) == 0) {
					fprintf (stderr, "-c option: must have at least one valid character\n");
					exit (-1);
				}
				/*
				** most of the time, grabchars can be
				** called safely with things like
				** "a-z", because we can check to
				** see if we need to add brackets...
				*/
				if (valid_chars[0] != '[' && 
				valid_chars[strlen (valid_chars) - 1] != ']')
					sprintf (valid_chars, "%c%s%c",
						'[', optarg, ']');

				if ((re_error = re_comp (valid_chars))
								!= NULL) {
					fprintf (stderr,
						"-c option: %s\n", re_error);
					exit (-1);
				}
				break;
			case 'd':
				flags->dflt = 1;
				strcpy (default_string, optarg);
				if (strlen (optarg) == 0) {
				fprintf (stderr, "-d option: must have at least one character for default\n");
					exit (-1);
				}
				break;
			case 'e':
				outfile = stderr;
				otherout = stdout;
				break;
			case 'f':
				flags->flush = 1;
				break;
			case 'n':
				how_many = atoi (optarg);
				if (how_many <= 0) {
					fprintf (stderr, "-n option: number of characters to read must be greater than zero\n");
					exit (-1);
				}
				break;
			case 'p':
				fprintf (stdout, "%s", optarg);
				break;
			case 'q':
				fprintf (stderr, "%s", optarg);
				break;
			case 'r':
				flags->ret_key = 1;
				break;
			case 's':
				flags->silent = 1;
				break;
			case 't':
				timeout = atoi (optarg);
				if (timeout <= 0) {
					fprintf (stderr, "-t option: number of seconds to timeout must be greater than zero\n");
					exit (-1);
				}

				/*
				** we must have some valid time >0 seconds to
				** get here, so we'll set an alarm...
				*/
				(SIGRET) signal (SIGALRM, overtime);
				alarm ((unsigned int) timeout);
				break;
			case 'E':	/* DV: honor erase/kill flag */
#ifdef DV_ERASE
				flags->erase = 1;
#else
				fprintf (stderr, "-E is disabled\n");
				exit (-1);
#endif
				break;

				/*
				** upper/lower case mapping...if both
				** are specified on the command line,
				** the last one wins out for sanity's
				** sake...
				*/
			case 'L':
				flags->lower = 1;
				flags->upper = 0;
				break;
			case 'U':
				flags->upper = 1;
				flags->lower = 0;
				break;

				/*
				** I bet I could leave out "default", but
				** I also bet that all getopt () routines
				** are not created equal, so in it stays!
				*/
			case '?':
			default:
				i = 0;
				while (usage_statement[i])
					puts (usage_statement[i++]);
				exit (-1);
		}
	}

	/* we're still here, really running...now change the tty... */
	init_term ();

#ifdef DV_ERASE
	/* DV: malloc (okay, well calloc) space for the erase buffer */
	if (flags->erase) {
		/* We can't do it up in the switch, because we don't know
		** how many is how_many
		*/
		erase_buf = (char *)calloc (1, how_many);
		if (erase_buf == NULL) {
			fprintf (stderr,
			"Error:  Couldn't malloc space for erase buffer\n");
			exit_stat = -1;
			lets_go();
		}
	}
#endif

	for (num_read = 0; num_read < how_many; num_read++) {
		ch = getchar ();

		/* use default_string, this does *not* return */
		if (ch == '\n' && flags->dflt && num_read == 0)
			handle_default ();

		/*
		** set by -r, a RETURN key gets us out (use with -n)
		** suggested by David Vezie
		*/
		if (ch == '\n' && flags->ret_key)
			break;

		/*	filter chars... */
		/*
		** known bug.... need to check the erase/kill keys somehow
		** the workaround is to make them part of the allowable
		** string of chars that can be input
		*/
		if (flags->check) {
			sprintf (check_str, "%c", ch);
			if (re_exec (check_str) != 1) {
				num_read--;
				continue;
			}
		}

		/* upper/lower case mapping, added 12/12/88 */
		if (flags->upper)
			ch = mk_upper (ch);
		if (flags->lower)
			ch = mk_lower (ch);

		/*
		** if we're just looking for a return status
		** then have flags->silent set (-s)
		**
		** DV: Also, we don't want to output yet,
		** if we're processing erase/kill charfacters.
		*/
#ifdef DV_ERASE
		if (! flags->silent && ! flags->erase) {
#else
		if (! flags->silent) { 
#endif
			putc (ch, outfile);
			if (flags->both)
				putc (ch, otherout);
		}

#ifdef DV_ERASE
		if (flags->erase)
			handle_erase (ch, &num_read);
#endif
	}

#ifdef DV_ERASE
	if (flags->erase) {
		fprintf (outfile, "%s", erase_buf);
		if (flags->both)
			fprintf (otherout, "%s", erase_buf);
	}
#endif

	exit_stat = num_read;
	lets_go ();
}
