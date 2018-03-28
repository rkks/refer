/*
	editor front-end (bidirectional pipe)
	AUP2, Sec. 6.06

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
/*
	Sec. 6.06
*/
#include "defs.h"

/*[edinvoke1]*/
static FILE *sndfp, *rcvfp;

static bool edinvoke(void)
{
	int pfd[2];
	pid_t pid;

	ec_neg1( pipe(pfd) )
	switch (fork()) {
	case -1:
		EC_FAIL
	case 0:
		ec_neg1( dup2(pfd[0], STDIN_FILENO) )
		ec_neg1( dup2(pfd[0], STDOUT_FILENO) )
		ec_neg1( close(pfd[0]) )
		execlp("ed", "ed", "-", NULL);
		EC_FAIL
	}
	ec_null( sndfp = fdopen(pfd[1], "w") )
	ec_null( rcvfp = fdopen(pfd[1], "r") )
	return true;

EC_CLEANUP_BGN
	if (pid == 0) {
		EC_FLUSH("edinvoke");
		_exit(EXIT_FAILURE);
	}
	return false;
EC_CLEANUP_END
}
/*[edsnd]*/
static bool edsnd(const char *s)
{
	ec_eof( fputs(s, sndfp) )
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}

static bool edrcv(char *s, size_t smax)
{
	ec_null( fgets(s, smax, rcvfp) )
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}

static bool turnaround(void)
{
	ec_false( edsnd("r end-of-file\n") )
	ec_eof( fflush(sndfp) )
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[rcvall]*/
static bool rcvall(void)
{
	char s[200];

	ec_false( turnaround() )
	while (true) {
		ec_false( edrcv(s, sizeof(s)) )
		if (strcmp(s, "?end-of-file\n") == 0)
			break;
		printf("%s", s);
	}
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[prompt]*/
static bool prompt(const char *msg, char *result, size_t resultmax,
  bool *eofp)
{
	char *p;

	printf("\n%s? ", msg);
	if (fgets(result, resultmax, stdin) == NULL) {
		if (ferror(stdin))
			EC_FAIL
		*eofp = true;
	}
	else {
		if ((p = strrchr(result, '\n')) != NULL)
			*p = '\0';
		*eofp = false;
	}
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[main]*/
int main(void)
{
	char s[100], line[200];
	bool eof;

	ec_false( prompt("File", s, sizeof(s), &eof) )
	if (eof)
		exit(EXIT_SUCCESS);
	ec_false( edinvoke() )
	snprintf(line, sizeof(line), "e %s\n", s);
	ec_false( edsnd(line) )
	ec_false( rcvall() );
	while (true) {
		ec_false( prompt("Search pattern", s, sizeof(s), &eof) )
		if (eof)
			break;
		snprintf(line, sizeof(line), "g/%s/p\n", s);
		ec_false( edsnd(line) )
		ec_false( rcvall() );
	}
	ec_false( edsnd("q\n") )
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
/*[]*/

