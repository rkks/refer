/*
	shells, version 1, 2, and 3
	AUP2, Sec. 5.04, 5.06, 5.10

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
#include "builtin.h"
#include "statuspr.h"
#include <sys/wait.h>

//#define WANT_SHELL_V1
//#define WANT_SHELL_V2
#define WANT_SHELL_V3

/*[macros]*/
#define MAXARG 20
#define MAXLINE 200
/*[]*/
#ifdef WANT_SHELL_V1
/*[execute]*/
static void execute(int argc, char *argv[])
{
	execvp(argv[0], argv);
	printf("Can't execute\n");
}
/*[]*/
#endif /* WANT_SHELL_V1 */
#ifdef WANT_SHELL_V2
#define execute execute2
/*[execute2]*/
static void execute2(int argc, char *argv[])
{
	pid_t pid;

	switch (pid = fork()) {
	case -1: /* parent (error) */
		EC_FAIL
	case 0: /* child */
		execvp(argv[0], argv);
		EC_FAIL
	default: /* parent */
		ec_neg1( wait(NULL) )
	}
	return;

EC_CLEANUP_BGN
	EC_FLUSH("execute2")
	if (pid == 0)
		_exit(EXIT_FAILURE);
EC_CLEANUP_END
}
/*[]*/
#endif /* WANT_SHELL_V2 */
#ifdef WANT_SHELL_V3
#define execute execute3
/*[execute3]*/
static void execute3(int argc, char *argv[])
{
	pid_t pid;
	int status;

	switch (pid = fork()) {
	case -1: /* parent (error) */
		EC_FAIL
	case 0: /* child */
		execvp(argv[0], argv);
		EC_FAIL
	default: /* parent */
		ec_neg1( waitpid(pid, &status, 0) )
		display_status(pid, status);
	}
	return;

EC_CLEANUP_BGN
	EC_FLUSH("execute3")
	if (pid == 0)
		_exit(EXIT_FAILURE);
EC_CLEANUP_END
}
/*[]*/
#endif /* WANT_SHELL_V3 */
/*[getargs]*/
static bool getargs(int *argcp, char *argv[], int max, bool *eofp)
{
	static char cmd[MAXLINE];
	char *cmdp;
	int i;

	*eofp = false;
	if (fgets(cmd, sizeof(cmd), stdin) == NULL) {
		if (ferror(stdin))
			EC_FAIL
		*eofp = true;
		return false;
	}
	if (strchr(cmd, '\n') == NULL) {
		/* eat up rest of line */
		while (true) {
			switch (getchar()) {
			case '\n':
				break;
			case EOF:
				if (ferror(stdin))
					EC_FAIL
			default:
				continue;
			}
			break;
		}
		printf("Line too long -- command ignored\n");
		return false;
	}
	cmdp = cmd;
	for (i = 0; i < max; i++) {
		if ((argv[i] = strtok(cmdp, " \t\n")) == NULL)
			break;
		cmdp = NULL; /* tell strtok to keep going */
	}
	if (i >= max) {
		printf("Too many args -- command ignored\n");
		return false;
	}
	*argcp = i;
	return true;

EC_CLEANUP_BGN
	EC_FLUSH("getargs")
	return false;
EC_CLEANUP_END
}
/*[main]*/
int main(void)
{
	char *argv[MAXARG];
	int argc;
	bool eof;

	while (true) {
		printf("@ ");
		if (getargs(&argc, argv, MAXARG, &eof) && argc > 0) {
			if (strchr(argv[0], '=') != NULL)
				asg(argc, argv);
			else if (strcmp(argv[0], "set") == 0)
				set(argc, argv);
			else
				execute(argc, argv);
		}
		if (eof)
			exit(EXIT_SUCCESS);

	}
}
/*[]*/

