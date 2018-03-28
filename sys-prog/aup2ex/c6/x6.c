/*
	Miscellaneous examples for Chap. 6
	AUP2, Chap. 6

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
#include <sys/wait.h>

/*[pipetest]*/
void pipetest(void)
{
	int pfd[2];
	ssize_t nread;
	char s[100];

	ec_neg1( pipe(pfd) )
	ec_neg1( write(pfd[1], "hello", 6) )
	ec_neg1( nread = read(pfd[0], s, sizeof(s)) )
	if (nread == 0)
		printf("EOF\n");
	else
		printf("read %ld bytes: %s\n", (long)nread, s);
	return;

EC_CLEANUP_BGN
	EC_FLUSH("pipetest");
EC_CLEANUP_END
}
/*[]*/

/*[pipewrite]*/
void pipewrite(void)
{
	int pfd[2];
	char fdstr[10];

	ec_neg1( pipe(pfd) )
	switch (fork()) {
	case -1:
		EC_FAIL
	case 0: /* child */
		ec_neg1( close(pfd[1]))
		snprintf(fdstr, sizeof(fdstr), "%d", pfd[0]);
		execlp("./piperead", "piperead", fdstr, NULL);
		EC_FAIL
	default: /* parent */
		ec_neg1( close(pfd[0]) )
		ec_neg1( write(pfd[1], "hello", 6) )
	}
	return;

EC_CLEANUP_BGN
	EC_FLUSH("pipewrite");
EC_CLEANUP_END
}
/*[]*/

/*[pipewrite2bug]*/
void pipewrite2bug(void) /* has a bug */
{
	int pfd[2];
	pid_t pid;

	ec_neg1( pipe(pfd) )
	switch (pid = fork()) {
	case -1:
		EC_FAIL
	case 0: /* child */
		ec_neg1( dup2(pfd[0], STDIN_FILENO) )
		ec_neg1( close(pfd[0]))
		ec_neg1( close(pfd[1]))
		execlp("cat", "cat", NULL);
		EC_FAIL
	default: /* parent */
		ec_neg1( close(pfd[0]) )
		ec_neg1( write(pfd[1], "hello", 6) )
		ec_neg1( waitpid(pid, NULL, 0) )
	}
	return;

EC_CLEANUP_BGN
	EC_FLUSH("pipewrite2bug");
EC_CLEANUP_END
}
/*[]*/

/*[pipewrite2]*/
void pipewrite2(void)
{
	int pfd[2];
	pid_t pid;

	ec_neg1( pipe(pfd) )
	switch (pid = fork()) {
	case -1:
		EC_FAIL
	case 0: /* child */
		ec_neg1( dup2(pfd[0], STDIN_FILENO) )
		ec_neg1( close(pfd[0]))
		ec_neg1( close(pfd[1]))
		execlp("cat", "cat", NULL);
		EC_FAIL
	default: /* parent */
		ec_neg1( close(pfd[0]) )
		ec_neg1( write(pfd[1], "hello", 6) )
		ec_neg1( close(pfd[1]) )
		ec_neg1( waitpid(pid, NULL, 0) )
	}
	return;

EC_CLEANUP_BGN
	EC_FLUSH("pipewrite2");
EC_CLEANUP_END
}
/*[]*/

/*[who_wc]*/
void who_wc(void)
{
	int pfd[2];
	pid_t pid1, pid2;

	ec_neg1( pipe(pfd) )
	switch (pid1 = fork()) {
	case -1:
		EC_FAIL
	case 0: /* first child */
		ec_neg1( dup2(pfd[1], STDOUT_FILENO) )
		ec_neg1( close(pfd[0]))
		ec_neg1( close(pfd[1]))
		execlp("who", "who", NULL);
		EC_FAIL
	}
	/* parent */
	switch (pid2 = fork()) {
	case -1:
		EC_FAIL
	case 0: /* second child */
		ec_neg1( dup2(pfd[0], STDIN_FILENO) )
		ec_neg1( close(pfd[0]))
		ec_neg1( close(pfd[1]))
		execlp("wc", "wc", "-l", NULL);
		EC_FAIL
	}
	/* still the parent */
	ec_neg1( close(pfd[0]) )
	ec_neg1( close(pfd[1]) )
	ec_neg1( waitpid(pid1, NULL, 0) )
	ec_neg1( waitpid(pid2, NULL, 0) )
	return;

EC_CLEANUP_BGN
	EC_FLUSH("who_wc");
EC_CLEANUP_END
}
/*[]*/

/*[who_wc2]*/
void who_wc2(void)
{
	int pfd[2];
	pid_t pid1, pid2;

	ec_neg1( pipe(pfd) )
	switch (pid1 = fork()) {
	case -1:
		EC_FAIL
	case 0: /* child */
		switch (pid2 = fork()) {
		case -1:
			EC_FAIL
		case 0: /* grandchild */
			ec_neg1( dup2(pfd[0], STDIN_FILENO) )
			ec_neg1( close(pfd[0]))
			ec_neg1( close(pfd[1]))
			execlp("wc", "wc", "-l", NULL);
			EC_FAIL
		}
		/* still the child */
		ec_neg1( dup2(pfd[1], STDOUT_FILENO) )
		ec_neg1( close(pfd[0]))
		ec_neg1( close(pfd[1]))
		execlp("who", "who", NULL);
		EC_FAIL
	}
	/* parent */
	ec_neg1( close(pfd[0]) )
	ec_neg1( close(pfd[1]) )
	ec_neg1( waitpid(pid1, NULL, 0) )
	return;

EC_CLEANUP_BGN
	EC_FLUSH("who_wc2");
EC_CLEANUP_END
}
/*[]*/

/*[fsort0]*/
void fsort0(void)  /* wrong */
{
	int pfd[2], fd;
	ssize_t nread;
	pid_t pid;
	char buf[512];

	ec_neg1( pipe(pfd) )
	ec_neg1( pid = fork() )
	if (pid == 0) { /* child */
		ec_neg1( dup2(pfd[0], STDIN_FILENO) )
		ec_neg1( close(pfd[0]) )
		ec_neg1( dup2(pfd[1], STDOUT_FILENO) )
		ec_neg1( close(pfd[1]) )
		execlp("sort", "sort", NULL);
		EC_FAIL
	}
	/* parent */
	ec_neg1( fd = open("datafile", O_RDONLY) )
	while (true) {
		ec_neg1( nread = read(fd, buf, sizeof(buf)) )
		if (nread == 0)
			break;
		ec_neg1( write(pfd[1], buf, nread) )
	}
	ec_neg1( close(fd) )
	ec_neg1( close(pfd[1]) )
	while (true) {
		ec_neg1( nread = read(pfd[0], buf, sizeof(buf)) )
		if (nread == 0)
			break;
		ec_neg1( write(STDOUT_FILENO, buf, nread) )
	}
	ec_neg1( close(pfd[0]) )
	ec_neg1( waitpid(pid, NULL, 0) )
	return;

EC_CLEANUP_BGN
	EC_FLUSH("fsort0");
EC_CLEANUP_END
}
/*[]*/

/*[fsort]*/
void fsort(void)
{
	int pfdout[2], pfdin[2], fd;
	ssize_t nread;
	pid_t pid;
	char buf[512];

	ec_neg1( pipe(pfdout) )
	ec_neg1( pipe(pfdin) )
	ec_neg1( pid = fork() )
	if (pid == 0) { /* child */
		ec_neg1( dup2(pfdout[0], STDIN_FILENO) )
		ec_neg1( close(pfdout[0]) )
		ec_neg1( close(pfdout[1]) )
		ec_neg1( dup2(pfdin[1], STDOUT_FILENO) )
		ec_neg1( close(pfdin[0]) )
		ec_neg1( close(pfdin[1]) )
		execlp("sort", "sort", NULL);
		EC_FAIL
	}
	/* parent */
	ec_neg1( close(pfdout[0]) )
	ec_neg1( close(pfdin[1]) )
	ec_neg1( fd = open("datafile", O_RDONLY) )
	while (true) {
		ec_neg1( nread = read(fd, buf, sizeof(buf)) )
		if (nread == 0)
			break;
		ec_neg1( write(pfdout[1], buf, nread) )
	}
	ec_neg1( close(fd) )
	ec_neg1( close(pfdout[1]) )
	while (true) {
		ec_neg1( nread = read(pfdin[0], buf, sizeof(buf)) )
		if (nread == 0)
			break;
		ec_neg1( write(STDOUT_FILENO, buf, nread) )
	}
	ec_neg1( close(pfdin[0]) )
	ec_neg1( waitpid(pid, NULL, 0) )
	return;

EC_CLEANUP_BGN
	EC_FLUSH("fsort");
EC_CLEANUP_END
}
/*[]*/

static void fpathconf_test(void)
{
/*[fpathconf_test]*/
int pfd[2];
long v;

ec_neg1( pipe(pfd) )
errno = 0;
v = fpathconf(pfd[0], _PC_PIPE_BUF);
if (errno != 0)
	EC_FAIL
else if (v == -1)
	printf("No limit for PIPE_BUF\n");
else
	printf("PIPE_BUF = %ld\n", v);
/*[]*/
return;

EC_CLEANUP_BGN
	EC_FLUSH("fpathconf_test")
EC_CLEANUP_END
}
/*[pipe_access_mode]*/
void pipe_access_mode(void)
{
	int pfd[2], flags, i;

	ec_neg1( pipe(pfd) )
	for (i = 0; i < 2; i++) {
		ec_neg1( flags = fcntl(pfd[i], F_GETFL) )
		if ((flags & O_ACCMODE) == O_RDONLY)
			printf("pfd[%d] O_RDONLY\n", i);
		if ((flags & O_ACCMODE) == O_WRONLY)
			printf("pfd[%d] O_WRONLY\n", i);
		if ((flags & O_ACCMODE) == O_RDWR)
			printf("pfd[%d] O_RDWR\n", i);
	}
	return;

EC_CLEANUP_BGN
	EC_FLUSH("pipe_access_mode")
EC_CLEANUP_END
}
/*[]*/

int main(void)
{
	char buf[10];

	while (true) {
		printf("Choose example to run:\n");
		printf("\t1: pipetest\n");
		printf("\t2: pipewrite\n");
		printf("\t3: pipewrite2bug (hangs)\n");
		printf("\t4: pipewrite2\n");
		printf("\t5: who_wc\n");
		printf("\t6: who_wc2\n");
		printf("\t7: fsort0 (wrong)\n");
		printf("\t8: fsort\n");
		printf("\t9: fpathconf\n");
		printf("\ta: pipe_access_mode\n");
		printf("\tp: ps\n");
		printf("\tq: quit\n");
		printf("\nChoice? ");

		if (fgets(buf, sizeof(buf), stdin) != NULL)
			switch (buf[0]) {
			case '1':
				pipetest();
				continue;
			case '2':
				pipewrite();
				continue;
			case '3':
				pipewrite2bug();
				continue;
			case '4':
				pipewrite2();
				continue;
			case '5':
				who_wc();
				continue;
			case '6':
				who_wc2();
				continue;
			case '7':
				fsort0();
				continue;
			case '8':
				fsort();
				continue;
			case '9':
				fpathconf_test();
				continue;
			case 'a':
				pipe_access_mode();
				continue;
			case 'p':
				system("ps");
				continue;
			case 'q':
				break;
			default:
				printf("Invalid response\n");
				continue;
			}
		break;
	}
	exit(EXIT_SUCCESS);
}
