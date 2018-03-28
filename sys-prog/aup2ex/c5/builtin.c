/*
	asg and set built-in commands (used in shells)
	AUP2, Sec. 5.04

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
#include "setenv.h"

/*[asg]*/
void asg(int argc, char *argv[])
{
	char *name, *val;

	if (argc != 1)
		printf("Extra args\n");
	else {
		name = strtok(argv[0], "=");
		val = strtok(NULL, ""); /* get all that's left */
		if (name == NULL || val == NULL)
			printf("Bad command\n");
		else
			ec_neg1( setenv(name, val, true) )
	}
	return;

EC_CLEANUP_BGN
	EC_FLUSH("asg")
EC_CLEANUP_END
}
/*[set]*/
extern char **environ;

void set(int argc, char *argv[])
{
	int i;

	if (argc != 1)
		printf("Extra args\n");
	else
		for (i = 0; environ[i] != NULL; i++)
			printf("%s\n", environ[i]);
}
/*[]*/
