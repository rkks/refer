#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stropts.h>
#include <sys/strlog.h>
#include <sys/stream.h>
#include <sys/log.h>
#include <string.h>
#include "ssp.h"

int main (void)
{
	int fd;
	struct strbuf control;
	struct strbuf data;
	struct log_ctl log;
	char *message = "Danger Will Robinson!";

	if ((fd = open ("/dev/log", O_WRONLY)) == -1)
		err_msg ("Can't open /dev/log");

	control.maxlen = sizeof (log);
	control.len = sizeof (log);
	control.buf = (caddr_t) &log;

	data.maxlen = strlen (message);
	data.len = strlen (message);
	data.buf = message;

	log.level = 42;
	log.flags = SL_TRACE | SL_ERROR | SL_FATAL;

	if (putmsg (fd, &control, &data, 0) == -1)
		err_msg ("putmsg failed");

	return (0);
}
