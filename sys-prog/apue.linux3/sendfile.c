#include	"lprps.h"

void
send_file(void)		/* called by main() to copy stdin to printer */
{
	int		c;

	init_input(1);
	set_intr();				/* we catch SIGINT */

	while ( (c = getchar()) != EOF)		/* main loop of program */
		out_char(c);		/* output each character */
	out_char(EOF);			/* output final buffer */

	block_write(&eofc, 1); 	/* send EOF to printer */
	proc_upto_eof(0);		/* wait for printer to send EOF back */
}
