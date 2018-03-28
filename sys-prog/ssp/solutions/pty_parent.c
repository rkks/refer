#include <stdio.h>
#include <unistd.h>
#include <stropts.h>
#include <termios.h>
#include <signal.h>
#include <string.h>
#include "ssp.h"

int main (void)
{
	int master_fd;
	pid_t pid;
	struct winsize pty_winsize;
	char buf [LINE_LEN];

	if ((pid = pty_fork (&master_fd, NULL, NULL)) == -1)
		err_msg ("pty_fork failed");

	if (pid == 0) {
		if (execl ("./pty_child", "pty_child", NULL) == -1)
			err_msg ("execl failed");
	}

	sleep (1);

	if (kill (-pid, SIGTERM) == -1)
		err_msg ("kill failed");

	memset (buf, 0, LINE_LEN);
	if (read (master_fd, buf, LINE_LEN) == -1)
		err_msg ("read failed");
	printf ("From pty_child: %s", buf);

	pty_winsize.ws_row = 20;
	pty_winsize.ws_col = 40;

	if (ioctl (master_fd, TIOCSWINSZ, &pty_winsize) == -1)
		err_msg ("ioctl failed");

	memset (buf, 0, LINE_LEN);
	if (read (master_fd, buf, LINE_LEN) == -1)
		err_msg ("read failed");
	printf ("From pty_child: %s", buf);

	return (0);
}
