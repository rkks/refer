#include	"lprps.h"
#include	<fcntl.h>
#include	<termios.h>

static int		block_flag = 1;		/* default is blocking I/O */

void
set_block(void)		/* turn off nonblocking flag */
{					/* called only by block_write() below */
	int		val;

	if (block_flag == 0) {
		if ( (val = fcntl(psfd, F_GETFL, 0)) < 0)
			log_sys("set_block: fcntl F_GETFL error");
		val &= ~O_NONBLOCK;
		if (fcntl(psfd, F_SETFL, val) < 0)
			log_sys("set_block: fcntl F_SETFL error");

		block_flag = 1;
	}
}

void
set_nonblock(void)	/* set descriptor nonblocking */
{
	int		val;

	if (block_flag) {
		if ( (val = fcntl(psfd, F_GETFL, 0)) < 0)
			log_sys("set_nonblock: fcntl F_GETFL error");
		val |= O_NONBLOCK;
		if (fcntl(psfd, F_SETFL, val) < 0)
			log_sys("set_nonblock: fcntl F_SETFL error");

		block_flag = 0;
	}
}

void
block_write(const char *buf, int n)
{
	set_block();
	if (write(psfd, buf, n) != n)
		log_sys("block_write: write error");
}

void
tty_flush(void)		/* flush (empty) tty input and output queues */
{
	if (tcflush(psfd, TCIOFLUSH) < 0)
		log_sys("tty_flush: tcflush error");
}
void
tty_open(void)
{
	struct termios	term;

	if ( (psfd = open(DEF_DEVICE, O_RDWR)) < 0)
		log_sys("tty_open: open error");

	if (tcgetattr(psfd, &term) < 0)		/* fetch attributes */
		log_sys("tty_open: tcgetattr error");
	term.c_cflag  = CS8 |			/* 8-bit data */
					CREAD |			/* enable receiver */
					CLOCAL;			/* ignore modem status lines */
									/* no parity, 1 stop bit */
	term.c_oflag &= ~OPOST;			/* turn off post processing */
	term.c_iflag  = IXON | IXOFF |	/* Xon/Xoff flow control */
					IGNBRK |		/* ignore breaks */
					ISTRIP |		/* strip input to 7 bits */
					IGNCR;			/* ignore received CR */
	term.c_lflag  = 0;		/* everything off in local flag:
							   disables canonical mode, disables
							   signal generation, disables echo */
	term.c_cc[VMIN]  = 1;	/* 1 byte at a time, no timer */
	term.c_cc[VTIME] = 0;
	cfsetispeed(&term, DEF_BAUD);
	cfsetospeed(&term, DEF_BAUD);
	if (tcsetattr(psfd, TCSANOW, &term) < 0)	/* set attributes */
		log_sys("tty_open: tcsetattr error");
}
