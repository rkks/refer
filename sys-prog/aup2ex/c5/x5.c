/*
	Environment examples
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
#include "defs.h"

#define main main1

/*[list-environ]*/
extern char **environ;

int main(void)
{
	int i;

	for (i = 0; environ[i] != NULL; i++)
		printf("%s\n", environ[i]);
	exit(EXIT_SUCCESS);
}
/*[]*/

#undef main
#define main main2

/*[getenv]*/
int main(void)
{
	char *s;

	s = getenv("LOGNAME");

	if (s == NULL)
		printf("variable not found\n");
	else
		printf("value is \"%s\"\n", s);
	exit(EXIT_SUCCESS);
}
/*[]*/

#undef main

#include "setenv.h"

int main(void)
{
	int i;

	for (i = 0; environ[i] != NULL; i++)
		printf("%s\n", environ[i]);
	ec_neg1( setenv("HOME", "abc", true) )
	for (i = 0; environ[i] != NULL; i++)
		printf("%s\n", environ[i]);
	ec_neg1( setenv("HOME", "This is a much larger value", true) )
	for (i = 0; environ[i] != NULL; i++)
		printf("%s\n", environ[i]);
	ec_neg1( setenv("HOME", "fox", false) )
	for (i = 0; environ[i] != NULL; i++)
		printf("%s\n", environ[i]);
	ec_neg1( unsetenv("HOME") )
	for (i = 0; environ[i] != NULL; i++)
		printf("%s\n", environ[i]);
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
