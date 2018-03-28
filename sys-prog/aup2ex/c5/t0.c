/*
	Non-blocking input
	AUP2, Sec. 5.17 (not in book)

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

int main(void)
{
	int i;
	char s[10];

	ec_false( setblock(fileno(stdin), false) )
	for (i = 0; ; i++) {
		if (fgets(s, sizeof(s), stdin) != NULL)
			break;
		sleep(1);
		printf("%d\n", i);
	}
	printf("stopping\n");
	return EXIT_SUCCESS;

EC_CLEANUP_BGN
	return EXIT_FAILURE;
EC_CLEANUP_END
}
