#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct entry {
	struct entry *next;
	pid_t pid;
	int fd;
} entry_t;

static entry_t *list_head = NULL;

static int add_entry (pid_t pid, int fd);
static pid_t del_entry (int fd);
static void close_other_pipes (void);

FILE *ssp_popen (const char *command, const char *mode)
{
	char *shell;
	FILE *fp;
	pid_t pid;
	int fd [2];
	int parent_fd;
	int child_fd;
	int std_io;

	if (((mode [0] != 'r') && (mode [0] != 'w')) || (mode [1] != 0))
		return (NULL);

	if (pipe (fd) == -1)
		return (NULL);

#ifndef _LP64
	if ((fd [0] >= 256) || (fd [1] >= 256)) {
		close (fd [0]);
		close (fd [1]);
		return (NULL);
	}
#endif

	parent_fd = (*mode == 'r') ? fd [0] : fd [1];
	child_fd = (*mode == 'r') ? fd [1] : fd [0];

	switch (pid = fork ()) {
		case -1:
			close (parent_fd);
			close (child_fd);
			return (NULL);

		case 0:
			close_other_pipes ();

			close (parent_fd);
			std_io = (*mode == 'r') ? 1 : 0;
			if (child_fd != std_io) {
				dup2 (child_fd, std_io);
				close (child_fd);
			}

			if (sysconf (_SC_XOPEN_VERSION) < 4)
				shell = "/bin/sh";
			else
				shell = "/usr/xpg4/bin/sh";

			if (access (shell, X_OK) == -1)
				_exit (127);

			execl (shell, "sh", "-c", command, NULL);
			_exit (1);

		default:	
			close (child_fd);

			if (add_entry (pid, parent_fd) == -1) {
				close (parent_fd);
				return (NULL);
			}

			if ((fp = fdopen (parent_fd, mode)) == NULL) {
				close (parent_fd);
				return (NULL);
			}

			return (fp);
	}
}

int ssp_pclose (FILE *fp)
{
	pid_t pid;
	int status;

	pid = del_entry (fileno (fp));

	fclose (fp);

	if (pid == -1)
		return (-1);

	while (waitpid (pid, &status, 0) == -1) {
		if (errno != EINTR)
			return (-1);
	}

	return (status);
}

static int add_entry (pid_t pid, int fd)
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

	new -> pid = pid;
	new -> fd = fd;
	new -> next = NULL;

	if (list_head == NULL)
		list_head = new;
	else
		prev -> next = new;

	return (0);
}

static pid_t del_entry (int fd)
{
	entry_t *prev;
	entry_t *current;
	pid_t pid;

	prev = list_head;
	current = list_head;

	while (current != NULL) {
		if (current -> fd == fd) {
			if (current == list_head)
				list_head = current -> next;
			else
				prev -> next = current -> next;

			pid = current -> pid;
			free (current);

			return (pid);
		}

		prev = current;
		current = current -> next;
	}

	return (-1);
}

static void close_other_pipes (void)
{
	entry_t *current;

	current = list_head;

	while (current != NULL) {
		close (current -> fd);
		current = current -> next;
	}
}
