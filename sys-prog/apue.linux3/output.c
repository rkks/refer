#include	"lprps.h"

static char	outbuf[OBSIZE];
static int	outcnt = OBSIZE;	/* #bytes remaining */
static char	*outptr = outbuf;

static void	out_buf(void);

/* Output a single character.
 * Called by main loop in send_file(). */

void
out_char(int c)
{
	if (c == EOF) {
		out_buf();		/* flag that we're all done */
		return;
	}

	if (outcnt <= 0)
		out_buf();		/* buffer is full, write it first */

	*outptr++ = c;		/* just store in buffer */
	outcnt--;
}

/* Output the buffer that out_char() has been storing into.
 * We have our own output function, so that we never block on a write
 * to the printer.  Each time we output our buffer to the printer,
 * we also see if the printer has something to send us.  If so,
 * we call proc_input_char() to process each character. */
static void
out_buf(void)
{
	char	*wptr, *rptr, ibuf[IBSIZE];
	int		wcnt, nread, nwritten;
	fd_set	rfds, wfds;

	FD_ZERO(&wfds);
	FD_ZERO(&rfds);
	set_nonblock();			/* don't want the write() to block */
	wptr = outbuf;			/* ptr to first char to output */
	wcnt = outptr - wptr;	/* #bytes to output */
	while (wcnt > 0) {
		FD_SET(psfd, &wfds);
		FD_SET(psfd, &rfds);
		if (intr_flag)
			handle_intr();
		while (select(psfd + 1, &rfds, &wfds, NULL, NULL) < 0) {
			if (errno == EINTR) {
				if (intr_flag)
					handle_intr();		/* no return */
			} else
				log_sys("out_buf: select error");
		}
		if (FD_ISSET(psfd, &rfds)) {		/* printer is readable */
			if ( (nread = read(psfd, ibuf, IBSIZE)) < 0)
				log_sys("out_buf: read error");
			rptr = ibuf;
			while (--nread >= 0)
				proc_input_char(*rptr++);
		}
		if (FD_ISSET(psfd, &wfds)) {		/* printer is writeable */
			if ( (nwritten = write(psfd, wptr, wcnt)) < 0)
				log_sys("out_buf: write error");
			wcnt -= nwritten;
			wptr += nwritten;
		}
	}
	outptr = outbuf;	/* reset buffer pointer and count */
	outcnt = OBSIZE;
}
