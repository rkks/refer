#include	"lprps.h"

static int	eof_count;
static int	ignore_input;
static enum parse_state {	/* state of parsing input from printer */
	NORMAL,
	HAD_ONE_PERCENT,
	HAD_TWO_PERCENT,
	IN_MESSAGE,
	HAD_RIGHT_BRACKET,
	HAD_RIGHT_BRACKET_AND_PERCENT
} parse_state;

/* Initialize our input machine. */

void
init_input(int job)
{
	in_job = job;		/* only true when send_file() calls us */
	parse_state = NORMAL;
	ignore_input = 0;
}

/* Read from the printer until we encounter an EOF.
 * Whether or not the input is processed depends on "ignore". */

void
proc_upto_eof(int ignore)
{
	int		ec;

	ignore_input = ignore;
	ec = eof_count;		/* proc_input_char() increments eof_count */
	while (ec == eof_count)
		proc_some_input();
}

/* Wait for some data then read it.
 * Call proc_input_char() for every character read. */

void
proc_some_input(void)
{
	char	ibuf[IBSIZE];
	char	*ptr;
	int		nread;
	fd_set	rfds;

	FD_ZERO(&rfds);
	FD_SET(psfd, &rfds);
	set_nonblock();
	if (intr_flag)
		handle_intr();
	if (alrm_flag)
		handle_alrm();
	while (select(psfd + 1, &rfds, NULL, NULL, NULL) < 0) {
		if (errno == EINTR) {
			if (alrm_flag)
				handle_alrm();		/* doesn't return */
			else if (intr_flag)
				handle_intr();		/* doesn't return */
		} else
			log_sys("proc_some_input: select error");
	}
	if ( (nread = read(psfd, ibuf, IBSIZE)) < 0)
		log_sys("proc_some_input: read error");
	else if (nread == 0)
		log_sys("proc_some_input: read returned 0");

	ptr = ibuf;
	while (--nread >= 0)
		proc_input_char(*ptr++);	/* process each character */
}

/* Called by proc_some_input() above after some input has been read.
 * Also called by out_buf() whenever asynchronous input appears. */

void
proc_input_char(int c)
{
	if (c == '\004') {
		eof_count++;	/* just count the EOFs */
		return;
	} else if (ignore_input)
		return;			/* ignore everything except EOFs */

	switch (parse_state) {		/* parse the input */
	case NORMAL:
		if (c == '%')
			parse_state = HAD_ONE_PERCENT;
		else
			mail_char(c);
		break;
	case HAD_ONE_PERCENT:
		if (c == '%')
			parse_state = HAD_TWO_PERCENT;
		else {
			mail_char('%'); mail_char(c);
			parse_state = NORMAL;
		}
		break;
	case HAD_TWO_PERCENT:
		if (c == '[') {
			msg_init();		/* message starting; init buffer */
			parse_state = IN_MESSAGE;
		} else {
			mail_char('%'); mail_char('%'); mail_char(c);
			parse_state = NORMAL;
		}
		break;
	case IN_MESSAGE:
		if (c == ']')
			parse_state = HAD_RIGHT_BRACKET;
		else
			msg_char(c);
		break;
	case HAD_RIGHT_BRACKET:
		if (c == '%')
			parse_state = HAD_RIGHT_BRACKET_AND_PERCENT;
		else {
			msg_char(']'); msg_char(c);
			parse_state = IN_MESSAGE;
		}
		break;
	case HAD_RIGHT_BRACKET_AND_PERCENT:
		if (c == '%') {
			parse_state = NORMAL;
			proc_msg();		/* we have a message; process it */
		} else {
			msg_char(']'); msg_char('%'); msg_char(c);
			parse_state = IN_MESSAGE;
		}
		break;
	default:
		abort();
	}
}
