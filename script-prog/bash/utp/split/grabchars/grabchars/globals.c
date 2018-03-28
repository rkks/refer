/*
**	$Header: globals.c,v 1.9 89/12/29 21:14:44 daniel grabchars_1_9 $
**
**	globals.c - declare global variables for grabchars
**
**	Dan Smith (daniel@island.uu.net), November 29, 1988
*/
#include <stdio.h>
#include "grabchars.h"	/* where we typedef FLAGS */

FILE *outfile, *otherout;
FLAG *flags;

int exit_stat;
char valid_chars[128], default_string[128];

#ifdef DV_ERASE
char *erase_buf;	/* DV: Malloc'ed later */
#endif

/*
**	this gets in the way sitting in grabchars.c, so I moved it here.
**	If you add anything, keep in mind that you're trying to squeeze
**	everything onto a 24 line window/terminal... The "-h" doc
**	is just a convenience, any option that grabchars doesn't already
**	use will give the help screen...
*/
char *usage_statement[] = {
"usage: grabchars		gets one keystroke",
"	-b			output to stdout and stderr",
"	-c<valid characters>	only <valid chars> are returned",
"	-d<char(s)>		default char or string to return",
"	-e			output to stderr instead of stdout",
"	-f			flush any previous input before reading",
"	-h			help screen",
"	-n<number>		number of characters to read",
"	-p<prompt>		prompt to help user",
"	-q<prompt>		prompt to help user (through stderr)",
"	-r			RETURN key exits (use with -n)",
"	-s			silent, just return status",
"	-t<seconds>		timeout after <seconds>",
"	-E			honor erase/kill characters", /* DV */
"	-U/-L			upper/lower case mapping on input",
"grabchars -c aeiou		get one of the vowels",
"grabchars -c i			get the letter 'i'",
"grabchars '-penter a letter '	print the prompt \"enter a letter \"",
"grabchars -n4			get four characters",
"grabchars -t2			timeout after two seconds",
" ",
"print a prompt and grab three characters...",
"grabchars -p 'enter three characters >> ' -n 3",
0
};

static char my_copyright[]=
"grabchars 1.9, (c) Dan Smith (daniel@island.uu.net), 1990";
