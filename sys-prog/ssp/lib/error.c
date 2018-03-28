#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <syslog.h>
#include "ssp.h"

int daemon_proc;

static void err_common (boolean_t flag, int level, const char *text,
	va_list args);

/*
 * Print an error message preceded by "text",
 * then dump core and exit.
 */
void err_dump (const char *text, ...)
{
	va_list arg;

	va_start (arg, text);
	err_common (B_TRUE, LOG_ERR, text, arg);
	va_end (arg);
	abort ();
}

/*
 * Print an error message preceded by "text",
 * then exit.
 */
void err_msg (const char *text, ...)
{
	va_list arg;

	va_start (arg, text);
	err_common (B_TRUE, LOG_ERR, text, arg);
	va_end (arg);
	exit (1);
}

/*
 * Print "text", then exit.
 */
void err_quit (const char *text, ...)
{
	va_list arg;

	va_start (arg, text);
	err_common (B_FALSE, LOG_ERR, text, arg);
	va_end (arg);
	exit (1);
}

/*
 * Non fatal error: print an error message
 * preceded by "text", then return.
 */
void err_ret (const char *text, ...)
{
	va_list arg;

	va_start (arg, text);
	err_common (B_TRUE, LOG_INFO, text, arg);
	va_end (arg);
}

/*
 * Print the message "text", then return.
 */
void log_msg (const char *text, ...)
{
	va_list arg;

	va_start (arg, text);
	err_common (B_FALSE, LOG_INFO, text, arg);
	va_end (arg);
}

/*
 * Common error routine.  Prints a variadic
 * error message, optionally followed by the
 * error text associated with errno.
 */
static void err_common (boolean_t flag, int level, const char *text,
	va_list args)
{
	int old_errno;
	int n;
	char buf [LINE_LEN];

	old_errno = errno;
#ifdef NEED_SNPRINTF
	n = vsprintf (buf, text, args);
#else
	n = vsnprintf (buf, sizeof (buf), text, args);
#endif
	if (flag)
		snprintf (buf + n, sizeof (buf) - n, ": %s", strerror (old_errno));
	strcat (buf, "\n");

	if (daemon_proc)
		syslog (level, buf);
	else {
		fflush (stdout);
		fprintf (stderr, "%s", buf);
		fflush (stderr);
	}
}
