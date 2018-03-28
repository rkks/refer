#include	"lprps.h"
#include	<ctype.h>

static char	msgbuf[MBSIZE];
static int	msgcnt;
static void	printer_flushing(void);

/* Called by proc_input_char() after it's seen the "%%[" that
 * starts a message. */

void
msg_init(void)
{
	msgcnt = 0;		/* count of chars in message buffer */
}

/* All characters received from the printer between the starting
 * %%[ and the terminating ]%% are placed into the message buffer
 * by proc_some_input().  This message will be examined by
 * proc_msg() below. */

void
msg_char(int c)
{
	if (c != '\0' && msgcnt < MBSIZE - 1)
		msgbuf[msgcnt++] = c;
}

/* This function is called by proc_input_char() only after the final
 * percent in a "%%[ <message> ]%%" has been seen.  It parses the
 * <message>, which consists of one or more "key: val" pairs.
 * If there are multiple pairs, "val" can end in a semicolon. */

void
proc_msg(void)
{
	char	*ptr, *key, *val;
	int		n;

	msgbuf[msgcnt] = 0;		/* null terminate message */
	for (ptr = strtok(msgbuf, ";"); ptr != NULL;
										ptr = strtok(NULL, ";")) {
		while (isspace(*ptr))
			ptr++;			/* skip leading spaces in key */
		key = ptr;

		if ( (ptr = strchr(ptr, ':')) == NULL)
			continue;	/* missing colon, something wrong, ignore */
		*ptr++ = '\0';	/* null terminate key (overwrite colon) */

		while (isspace(*ptr))
			ptr++;		/* skip leading spaces in val */
		val = ptr;
						/* remove trailing spaces in val */
		ptr = strchr(val, '\0');
		while (ptr > val && isspace(ptr[-1]))
			--ptr;
		*ptr = '\0';

		if (strcmp(key, "Flushing") == 0) {
			printer_flushing();		/* never returns */

		} else if (strcmp(key, "PrinterError") == 0) {
			log_msg("proc_msg: printer error: %s", val);

		} else if (strcmp(key, "Error") == 0) {
		  	mail_line("Your PostScript printer job "
					  "produced the error `%s'.\n", val);

		} else if (strcmp(key, "status") == 0) {
			if (strcmp(val, "idle") == 0)
				status = IDLE;
			else if (strcmp(val, "busy") == 0)
				status = BUSY;
			else if (strcmp(val, "waiting") == 0)
				status = WAITING;
			else
				status = UNKNOWN;	/* "printing", "PrinterError",
					   "initializing", or "printing test page". */

		} else if (strcmp(key, "OffendingCommand") == 0) {
		  	mail_line("The offending command was `%s'.\n", val);

		} else if (strcmp(key, "pagecount") == 0) {
			if (sscanf(val, "%d", &n) == 1 && n >= 0) {
		  		if (start_page < 0)
					start_page = n;
		  		else
					end_page = n;
			}
		}
	}
}

/* Called only by proc_msg() when the "Flushing" message
 * is received from the printer.  We exit. */

static void
printer_flushing(void)
{
	clear_intr();			/* don't catch SIGINT */

	tty_flush();			/* empty tty input and output queues */
	block_write(&eofc, 1);	/* send an EOF to the printer */

	proc_upto_eof(1);		/* this call won't be recursive,
							   since we specify to ignore input */
	get_page(&end_page);
	do_acct();
	exit(EXIT_SUCCESS);
}
