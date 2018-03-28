#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stropts.h>
#include "ssp.h"

static char control_buf [BUF_SIZE];
static char data_buf [BUF_SIZE];

int main (void)
{
	int n;
	int flags;
	struct strbuf control;
	struct strbuf data;

	control.buf = control_buf;
	control.maxlen = BUF_SIZE;
	data.buf = data_buf;
	data.maxlen = BUF_SIZE;

	for (;;) {
		flags = 0;

		n = getmsg (STDIN_FILENO, &control, &data, &flags);

		switch (n) {
			case -1:
				err_msg ("Can't getmsg");
				break;

			case MORECTL:
				log_msg ("getmsg returns MORECTL");
				break;

			case MOREDATA:
				log_msg ("getmsg returns MOREDATA");
				break;

			case MORECTL | MOREDATA:
				log_msg ("getmsg returns MORECTL | MOREDATA");
				break;
		}

		log_msg ("flags = %d, control.len = %d, data.len = %d", flags,
			control.len, data.len);

		if (data.len == 0)
			exit (0);
		if (data.len > 0)
			if (writen (STDOUT_FILENO, data.buf, data.len) != data.len)
				err_msg ("Can't write");
	}
}
