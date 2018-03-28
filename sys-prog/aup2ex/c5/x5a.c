/*
	Miscellaneous examples for Chap. 5
	AUP2, Chap. 5

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

/*[exec_path]*/
int exec_path(const char *path, char *const argv[], char *newargv[])
{
	int i;

	execv(path, argv);
	if (errno == ENOEXEC) {
		newargv[0] = argv[0];
		newargv[1] = (char *)path;
		i = 1;
		do {
			newargv[i + 1] = argv[i];
		} while (argv[i++] != NULL);
		return execv("/bin/sh", (char *const *)newargv);
	}
	return -1;
}
/*[execvp2]*/
int execvp2(const char *file, char *const argv[])
{
	char *s, *pathseq = NULL, *path = NULL, **newargv = NULL;
	int argc;

	for (argc = 0; argv[argc] != NULL; argc++)
		;
	/* If shell script, we'll need room for one additional arg and NULL. */
	ec_null( newargv = malloc((argc + 2) * sizeof(char *)) )
	s = getenv("PATH");
	if (strchr(file, '/') != NULL || s == NULL || s[0] == '\0')
		ec_neg1( exec_path(file, argv, newargv) )
	ec_null( pathseq = strdup(s) )
	/* Following line usually allocates too much */
	ec_null( path = malloc(strlen(file) + strlen(pathseq) + 2) )
	while ((s = strtok(pathseq, ":")) != NULL) {
		pathseq = NULL; /* tell strtok to keep going */
		if (s[0] == '\0')
			s = ".";
		strcpy(path, s);
		strcat(path, "/");
		strcat(path, file);
		exec_path(path, argv, newargv);
	}
	errno = ENOENT;
	EC_FAIL

EC_CLEANUP_BGN
	free(pathseq);
	free(path);
	free(newargv);
	return -1;
EC_CLEANUP_END
}
/*[]*/

/*[exectest]*/
void exectest(void)
{
	printf("The quick brown fox jumped over ");
	ec_neg1( execl("/bin/echo", "echo", "the", "lazy", "dogs.", NULL) )
	return;

EC_CLEANUP_BGN
	EC_FLUSH("exectest");
EC_CLEANUP_END
}
/*[]*/
/*[forkbuf]*/
void forkbuf(void)
{
	printf("Hello World!\n");
	ec_neg1(fork());
	exit(EXIT_SUCCESS);
	return;

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
/*[]*/

void junk(void)
{
	int fd, flags;
	long open_max;
	char *path = "/bin/echo", *arg0 = "hello", *arg1 = "gillian",
	  *arg2 = " and claire";

/*[close-before-exec]*/
errno = 0;
ec_neg1( open_max = sysconf(_SC_OPEN_MAX) )
printf("open_max = %ld\n", open_max);
for (fd = 0; fd < open_max - 1; fd++)
	(void)close(fd); /* ignore errors */
ec_neg1( execl(path, arg0, arg1, arg2, NULL) )
/*[close-on-exec]*/
for (fd = 0; fd < open_max - 1; fd++) {
	ec_neg1( flags = fcntl(fd, F_SETFD) )
	ec_neg1( fcntl(fd, F_SETFD, flags | FD_CLOEXEC) )
}
ec_neg1( execl(path, arg0, arg1, arg2, NULL) )
/*[]*/
	return;

EC_CLEANUP_BGN
	EC_FLUSH("junk");
EC_CLEANUP_END
}

/*[forktest]*/
void forktest(void)
{
	int pid;

	printf("Start of test\n");
	pid = fork();
	printf("Returned %d\n", pid);
}
/*[]*/

void gp(void)
{
/*5-23*/
	char file[10];
	int tempfd;

	sprintf(file, "TMP%ld", (long)getpid());
	if ((tempfd = open(file, O_RDWR | O_CREAT | O_TRUNC, 0666)) == -1)
		syserr("open");
/**/
}

int main(int argc, char *argv[])
{
	forktest();
	//junk();
	//exectest();
/*


	if (argc < 2) {
		fprintf(stderr, "Usage: x5a <cmd> [ <arg> ... ]\n");
		exit(EXIT_FAILURE);
	}
	execvp2(argv[1], &argv[1]);
	EC_FAIL
	//gp();
*/
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
