#include <termios.h>

static int saved_fd = -1;
static struct termios saved_term;
static enum {
	RESET,
	RAW,
	CBREAK
} term_state = RESET;

int tty_cbreak (int fd)
{
	struct termios term;

	if (tcgetattr (fd, &saved_term) == -1)
		return (-1);

	term = saved_term;
	term.c_lflag &= ~(ECHO | ICANON);
	term.c_cc [VMIN] = 1;
	term.c_cc [VTIME] = 0;
	if (tcsetattr (fd, TCSAFLUSH, &term) == -1)
		return (-1);

	term_state = CBREAK;
	saved_fd = fd;

	return (0);
}

int tty_raw (int fd)
{
	struct termios term;

	if (tcgetattr (fd, &saved_term) == -1)
		return (-1);

	term = saved_term;
	term.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	term.c_oflag &= ~(OPOST);
	term.c_cflag &= ~(CSIZE | PARENB);
	term.c_cflag |= CS8;
	term.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	term.c_cc [VMIN] = 1;
	term.c_cc [VTIME] = 0;
	if (tcsetattr (fd, TCSAFLUSH, &term) == -1)
		return (-1);

	term_state = RAW;
	saved_fd = fd;

	return (0);
}

int tty_reset (int fd)
{
	if (term_state == RESET)
		return (0);

	if (tcsetattr (fd, TCSAFLUSH, &saved_term) == -1)
		return (-1);

	term_state = RESET;
	saved_fd = -1;

	return (0);
}

void tty_atexit (void)
{
	if (saved_fd != -1)
		tty_reset (saved_fd);
}

struct termios *tty_getoldattr (void)
{
	return (&saved_term);
}
