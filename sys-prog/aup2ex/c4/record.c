/*
	Record and Playback Example
	AUP2, Sec. 4.10.2

	Copyright 2003 by Marc J. Rochkind. All rights reserved.
	May be copied only for purposes and under conditions described
	on the Web page www.basepath.com/aup/copyright.htm.

	The Example Files are provided "as is," without any warranty;
	without even the implied warranty of merchantability or fitness
	for a particular purpose. The author and his publisher are not
	responsible for any damages, direct or incidental, resulting
	from the use or non-use of these Example Files.

	The Example Files may contain defects, and some contain deliberate
	coding mistakes that were included for educational reasons.
	You are responsible for determining if and how the Example Files
	are to be used.

*/
#include "defs.h"
#include "ptutil.h"
#include "tc_setraw.h"
#include <sys/time.h>
#include <termios.h>

/*
	Only works if x >= y.
*/
/*[timeval_subtract]*/
static void timeval_subtract(const struct timeval *x,
  const struct timeval *y, struct timeval *diff)
{
	if (x->tv_sec == y->tv_sec || x->tv_usec >= y->tv_usec) {
		diff->tv_sec = x->tv_sec - y->tv_sec;
		diff->tv_usec = x->tv_usec - y->tv_usec;
	}
	else {
		diff->tv_sec = x->tv_sec - 1 - y->tv_sec;
		diff->tv_usec = 1000000 + x->tv_usec - y->tv_usec;
	}
}
/*[get_rel_time]*/
static void get_rel_time(struct timeval *tv_rel)
{
	static bool first = true;
	static struct timeval starttime;
	struct timeval tv;

	if (first) {
		first = false;
		(void)gettimeofday(&starttime, NULL);
	}
	(void)gettimeofday(&tv, NULL);
	timeval_subtract(&tv, &starttime, tv_rel);
}
/*[event-struct]*/
#define EVFILE "recording.tmp"
#define EVBUFSIZE 512

struct event {
	struct timeval e_time;
	unsigned e_datalen;
};
static int fd_ev = -1;
/*[ev_open]*/
static bool ev_open(void)
{
	ec_neg1( fd_ev = open(EVFILE, O_RDONLY) )
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[ev_creat]*/
static bool ev_creat(void)
{
	ec_neg1( fd_ev = open(EVFILE, O_WRONLY | O_CREAT | O_TRUNC,
	  PERM_FILE) )
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[ev_write]*/
static bool ev_write(char *data, unsigned datalen)
{
	struct event ev = { { 0 } };

	get_rel_time(&ev.e_time);
	ev.e_datalen = datalen;
	ec_neg1( writeall(fd_ev, &ev, sizeof(ev)) )
	ec_neg1( writeall(fd_ev, data, datalen) )
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[ev_read]*/
static bool ev_read(struct event *ev, char *data, unsigned datalen)
{
	ssize_t nread;

	ec_neg1( nread = read(fd_ev, ev, sizeof(*ev)) )
	if (nread != sizeof(*ev)) {
		errno = EIO;
		EC_FAIL
	}
	ec_neg1( nread = read(fd_ev, data, ev->e_datalen) )
	if (nread != ev->e_datalen) {
		errno = EIO;
		EC_FAIL
	}
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[ev_close]*/
static void ev_close(void)
{
	(void)close(fd_ev);
	fd_ev = -1;
}
/*[ev_sleep]*/
static bool ev_sleep(struct timeval *tv)
{
	struct timeval tv_rel, tv_diff;

	get_rel_time(&tv_rel);
	if (tv->tv_sec > tv_rel.tv_sec || (tv->tv_sec == tv_rel.tv_sec && tv->tv_usec >= tv_rel.tv_usec)) {
		timeval_subtract(tv, &tv_rel, &tv_diff);
		(void)sleep(tv_diff.tv_sec);
		ec_neg1( usleep(tv_diff.tv_usec) )
	}
	/* else we are already running late */
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[playback]*/
static bool playback(void)
{
	bool ok = false;
	struct event ev;
	char buf[EVBUFSIZE];
	struct termios tbuf, tbufsave;

	ec_neg1( tcgetattr(STDIN_FILENO, &tbuf) )
	tbufsave = tbuf;
	tbuf.c_lflag &= ~ECHO;
	ec_neg1( tcsetattr(STDIN_FILENO, TCSAFLUSH, &tbuf) )
	ec_false( ev_open() )
	while (true) {
		ec_false( ev_read(&ev, buf, sizeof(buf)) )
		if (ev.e_datalen == 0)
			break;
		ev_sleep(&ev.e_time);
		ec_neg1( writeall(STDOUT_FILENO, buf, ev.e_datalen) )
	}
	ec_neg1( write(STDOUT_FILENO, "\n", 1) )
	ok = true;
	EC_CLEANUP

EC_CLEANUP_BGN
	(void)tcdrain(STDOUT_FILENO);
	(void)sleep(1); /* Give the terminal a chance to respond. */
	(void)tcsetattr(STDIN_FILENO, TCSAFLUSH, &tbufsave);
	ev_close();
	return ok;
EC_CLEANUP_END
}
/*[]*/
/*
	Following function won't be explained until Chap. 6.
*/
/*[exec_redirected]*/
bool exec_redirected(const char *file, char *const argv[], int fd_stdin,
  int fd_stdout, int fd_stderr)
{
	if (fd_stdin != STDIN_FILENO)
		ec_neg1( dup2(fd_stdin, STDIN_FILENO) )
	if (fd_stdout != STDOUT_FILENO)
		ec_neg1( dup2(fd_stdout, STDOUT_FILENO) )
	if (fd_stderr != STDERR_FILENO)
		ec_neg1( dup2(fd_stderr, STDERR_FILENO) )
	if (fd_stdin != STDIN_FILENO)
		(void)close(fd_stdin);
	if (fd_stdout != STDOUT_FILENO)
		(void)close(fd_stdout);
	if (fd_stderr != STDERR_FILENO)
		(void)close(fd_stderr);
	ec_neg1( execvp(file, argv) )

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[main-1]*/
int main(int argc, char *argv[])
{
	bool ok = false;
	PTINFO *p = NULL;

	if (argc < 2) {
		fprintf(stderr, "Usage: record cmd ...\n       record -p\n");
		exit(EXIT_FAILURE);
	}
	if (strcmp(argv[1], "-p") == 0) {
		playback();
		ok = true;
		EC_CLEANUP
	}
/*[main-2]*/
	ec_null( p = pt_open_master() )
	switch (fork()) {
	case 0:
		ec_false( pt_open_slave(p) )
		ec_false( exec_redirected(argv[1], &argv[1], PT_GET_SLAVE_FD(p),
		  PT_GET_SLAVE_FD(p), PT_GET_SLAVE_FD(p)) )
		break;
	case -1:
		EC_FAIL
	}
	ec_false( ev_creat() )
	ec_false( pt_wait_master(p) )
/*[main-3]*/
	tc_setraw();
	while (true) {
		fd_set fd_set_read;
		char buf[EVBUFSIZE];
		ssize_t nread;

		FD_ZERO(&fd_set_read);
		FD_SET(STDIN_FILENO, &fd_set_read);
		FD_SET(PT_GET_MASTER_FD(p), &fd_set_read);
		ec_neg1( select(FD_SETSIZE, &fd_set_read, NULL, NULL, NULL) )
		if (FD_ISSET(STDIN_FILENO, &fd_set_read)) {
			ec_neg1( nread = read(STDIN_FILENO, &buf, sizeof(buf)) )
			ec_neg1( writeall(PT_GET_MASTER_FD(p), buf, nread) )
		}
		if (FD_ISSET(PT_GET_MASTER_FD(p), &fd_set_read)) {
			if ((nread = read(PT_GET_MASTER_FD(p), &buf,
			  sizeof(buf))) > 0) {
				ec_false( ev_write(buf, nread) )
				ec_neg1( writeall(STDOUT_FILENO, buf, nread) )
			}
			else if (nread == 0 || (nread == -1 && errno == EIO))
				break;
			else
				EC_FAIL
		}
	}
	ec_neg1( ev_write(NULL, 0) )
	fprintf(stderr,
	  "EOF or error reading stdin or master pseudo-terminal; exiting\n");
	ok = true;
	EC_CLEANUP
/*[main-4]*/
EC_CLEANUP_BGN
	if (p != NULL)
		(void)pt_close_master(p);
	tc_restore();
	ev_close();
	printf("\n");
	exit(ok ? EXIT_SUCCESS : EXIT_FAILURE);
EC_CLEANUP_END
}
/*[]*/
