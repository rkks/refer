#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/stropts.h>
#include <sys/resource.h>
#include "ssp.h"

#define CS_PIPE "/tmp/ssp_cs_open"

typedef struct entry {
	struct entry *next;
	uid_t uid;
	int fd;
} entry_t;

static entry_t *list_head = NULL;
static char *path;
static int oflag;

static void loop (void);
static void do_request (char *buf, ssize_t n, int pipe_fd);
static int buf2args (char *buf);
static int parse_args (int argc, char **argv);
static int add_entry (int fd, uid_t uid);
static int del_entry (int fd);

int main (int argc, char **argv)
{
	int c;
	int debug;
	int err_flag;

	debug = 0;
	err_flag = 0;
	opterr = 0;

	while ((c = getopt (argc, argv, "d")) != EOF) {
		switch (c) {
			case 'd':
				debug = 1;
				break;

			default:
				err_flag = 1;
				break;
		}
	}

	if (err_flag)
		err_quit ("Usage: opend2 [-d]");

	if (debug == 0)
		daemon_init ("opend2", 0);

	loop ();

	return (0);
}

static void loop (void)
{
	int i;
	ssize_t n;
	int listen_fd;
	int client_fd;
	int num_fds;
	char buf [LINE_LEN];
	uid_t uid;
	struct pollfd *fds;
	struct rlimit num_files;

	getrlimit (RLIMIT_NOFILE, &num_files);
	if ((fds = calloc (num_files.rlim_cur, sizeof (struct pollfd))) == NULL)
		err_msg ("Calloc failed");

	if ((listen_fd = srv_listen (CS_PIPE)) == -1)
		err_msg ("srv_listen failed");
	add_entry (listen_fd, 0);
	fds [0].fd = listen_fd;
	fds [0].events = POLLIN;
	num_fds = 0;

	for (;;) {
		if (poll (fds, num_fds + 1, INFTIM) == -1)
			err_msg ("poll failed");

		if (fds [0].revents & POLLIN) {
			if ((client_fd = srv_accept (listen_fd, &uid)) == -1)
				err_msg ("srv_accept failed");
			if (add_entry (client_fd, uid) == -1)
				err_msg ("add_entry failed");
			num_fds++;
			fds [num_fds].fd = client_fd;
			fds [num_fds].events = POLLIN;
			log_msg ("New connection: fd = %d, uid = %d", client_fd, uid);
		}

		for (i = 1; i <= num_fds; i++) {
			client_fd = fds [i].fd;
			if (client_fd == -1)
				continue;
			if (fds [i].revents & POLLHUP)
				goto hangup;
			else if (fds [i].revents & POLLIN) {
				if ((n = read (client_fd, buf, LINE_LEN)) == -1)
					err_msg ("read error on fd %d", client_fd);
				else if (n == 0) {
hangup:
					log_msg ("Connection closed: fd = %d", client_fd);
					del_entry (client_fd);
					fds [i].fd = -1;
					close (client_fd);
				}
				else
					do_request (buf, n, client_fd);
			}
		}
	}
}

static void do_request (char *buf, ssize_t n, int pipe_fd)
{
	int fd;

	if (buf [n - 1] != '\0') {
		send_err (pipe_fd, EINVAL);
		return;
	}

	log_msg ("Request: %s", buf);

	if (buf2args (buf) == -1) {
		send_err (pipe_fd, EINVAL);
		return;
	}

	if ((fd = open (path, oflag)) == -1) {
		send_err (pipe_fd, errno);
		return;
	}

	if (send_fd (pipe_fd, fd) == -1)
		err_msg ("send_fd failed");

	close (fd);
}

static int buf2args (char *buf)
{
	char *ptr;
	char *argv [_POSIX_ARG_MAX];
	int argc;

	if (strtok (buf, " \t\n") == NULL)
		return (-1);

	argc = 0;
	argv [argc] = buf;
	while ((ptr = strtok (NULL, " \t\n")) != NULL) {
		if (++argc >= (_POSIX_ARG_MAX - 1))
			return (-1);
		argv [argc] = ptr;
	}
	argv [++argc] = NULL;

	return (parse_args (argc, argv));
}

static int parse_args (int argc, char **argv)
{
	if ((argc != 3) || (strcmp (argv [0], "open") != 0))
		return (-1);

	path = argv [1];
	oflag = atoi (argv [2]);

	return (0);
}

static int add_entry (int fd, uid_t uid)
{
	entry_t *prev;
	entry_t *current;
	entry_t *new;

	prev = list_head;
	current = list_head;

	while (current != NULL) {
		prev = current;
		current = current -> next;
	}

	if ((new = malloc (sizeof (entry_t))) == NULL)
		return (-1);

	new -> uid = uid;
	new -> fd = fd;
	new -> next = NULL;

	if (list_head == NULL)
		list_head = new;
	else
		prev -> next = new;

	return (0);
}

static int del_entry (int fd)
{
	entry_t *prev;
	entry_t *current;

	prev = list_head;
	current = list_head;

	while (current != NULL) {
		if (current -> fd == fd) {
			if (current == list_head)
				list_head = current -> next;
			else
				prev -> next = current -> next;

			free (current);

			return (0);
		}

		prev = current;
		current = current -> next;
	}

	return (-1);
}
