/*
	setenv, getenv, and unsetenv
	AUP2, Sec. 5.02

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
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "setenv.h"

extern char **environ;

/*[setnew]*/
static int setnew(int i, const char *var, const char *val)
{
	char *s;

	if ((s = malloc(strlen(var) + 1 + strlen(val) + 1)) == NULL)
		return -1;
	strcpy(s, var);
	strcat(s, "=");
	strcat(s, val);
	/* possible memory leak with old value of environ[i] */
	environ[i] = s;
	return 0;
}
/*[setenv]*/
int setenv(const char *var, const char *val, int overwrite)
{
	int i;
	size_t varlen;
	char **e;

	if (var == NULL || val == NULL || var[0] == '\0' ||
	  strchr(var, '=') != NULL) {
		errno = EINVAL;
		return -1;
	}
	varlen = strlen(var);
	for (i = 0; environ[i] != NULL; i++)
		if (strncmp(environ[i], var, varlen) == 0 &&
		  environ[i][varlen] == '=')
			break;
	if (environ[i] == NULL) {
		if ((e = malloc((i + 2) * sizeof(char *))) == NULL)
			return -1;
		memcpy(e, environ, i * sizeof(char *));
		/* possible memory leaks with old pointer array */
		environ = e;
		environ[i + 1] = NULL;
		return setnew(i, var, val);
	}
	else {
		if (overwrite) {
			if (strlen(&environ[i][varlen + 1]) >= strlen(val)) {
				strcpy(&environ[i][varlen + 1], val);
				return 0;
			}
			return setnew(i, var, val);
		}
		return 0;
	}
}
/*[unsetenv]*/
int unsetenv(const char *var)
{
	int i, found = -1;
	size_t varlen;

	if (var == NULL || var[0] == '\0' || strchr(var, '=') != NULL) {
		errno = EINVAL;
		return -1;
	}
	varlen = strlen(var);
	for (i = 0; environ[i] != NULL; i++)
		if (strncmp(environ[i], var, varlen) == 0 &&
		  environ[i][varlen] == '=')
			found = i;
	if (found != -1)
		/* possible memory leak with old value of environ[found] */
		memmove(&environ[found], &environ[found + 1],
		  (i - found) * sizeof(char *));
	return 0;
}
/*[]*/
