#include	"lprps.h"

static void
sig_int(int signo)		/* SIGINT handler */
{
	intr_flag = 1;
	return;
}

/* This function is called after SIGINT has been delivered,
 * and the main loop has recognized it.  (It not called as
 * a signal handler, set_intr() above is the handler.) */

void
handle_intr(void)
{
	char	c;

	intr_flag = 0;
	clear_intr();			/* turn signal off */

	set_alrm(30);		/* 30 second timeout to interrupt printer */

	tty_flush();			/* discard any queued output */
	c = '\003';
	block_write(&c, 1);		/* Control-C interrupts the PS job */
	block_write(&eofc, 1);	/* followed by EOF */
	proc_upto_eof(1);		/* read & ignore up through EOF */

	clear_alrm();

	get_page(&end_page);
	do_acct();
	exit(EXIT_SUCCESS);		/* success since user lprm'ed the job */
}

void
set_intr(void)		/* enable signal handler */
{
	if (signal_intr(SIGINT, sig_int) == SIG_ERR)
		log_sys("set_intr: signal_intr error");
}

void
clear_intr(void)	/* ignore signal */
{
	if (signal(SIGINT, SIG_IGN) == SIG_ERR)
		log_sys("clear_intr: signal error");
}
