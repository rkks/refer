#include	"lprps.h"

/* Called by main() before printing job.
 * We send a Control-T to the printer to fetch its status.
 * If we timeout before reading the printer's status, something
 * is wrong. */

void
get_status(void)
{
	char	c;

	set_alrm(5);			/* 5 second timeout to fetch status */

	tty_flush();
	c = '\024';
	block_write(&c, 1);		/* send Control-T to printer */

	init_input(0);
	while (status == INVALID)
		proc_some_input();	/* wait for something back */

	switch (status) {
	case IDLE:		/* this is what we're looking for ... */
		clear_alrm();
		return;

	case WAITING:	/* printer thinks it's in the middle of a job */
		block_write(&eofc, 1);	/* send EOF to printer */
		sleep(5);
		exit(EXIT_REPRINT);

	case BUSY:
	case UNKNOWN:
		sleep(15);
		exit(EXIT_REPRINT);
	}
}
