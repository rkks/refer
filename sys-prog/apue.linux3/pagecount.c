#include	"lprps.h"

/* PostScript program to fetch the printer's pagecount.
 * Notice that the string returned by the printer:
 *		%%[ pagecount: N ]%%
 * will be parsed by proc_msg(). */

static char	pagecount_string[] =
	"(%%[ pagecount: ) print "	/* print writes to current output file */
	"statusdict begin pagecount end "	/* push pagecount onto stack */
	"20 string "		/* creates a string of length 20 */
	"cvs "				/* convert to string */
	"print "			/* write to current output file */
	"( ]%%) print "
	"flush\n";			/* flush current output file */

/* Read the starting or ending pagecount from the printer.
 * The argument is either &start_page or &end_page. */

void
get_page(int *ptrcount)
{
	set_alrm(30);			/* 30 second timeout to read pagecount */

	tty_flush();
	block_write(pagecount_string, sizeof(pagecount_string) - 1);
							/* send query to printer */
	init_input(0);
	*ptrcount = -1;
	while (*ptrcount < 0)
		proc_some_input();	/* read results from printer */

	block_write(&eofc, 1);	/* send EOF to printer */
	proc_upto_eof(0);		/* wait for EOF from printer */

	clear_alrm();
}
