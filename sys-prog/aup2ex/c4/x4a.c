/*
	tc_setraw test program
	AUP2, Sec. 4.05.10

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
#include <termios.h>
#include "tc_setraw.h"

int main(void)
{
	int c;

#if 0
	system("stty sane");
/*[tc_setraw-test]*/
setbuf(stdout, NULL);
printf("Initial attributes:\n");
system("stty | fold -s -w 60");
printf("\r\nRaw attributes:\n");
tc_setraw();
system("stty | fold -s -w 60");
tc_restore();
printf("\r\nRestored attributes:\n");
system("stty | fold -s -w 60");
/*[]*/
	system("stty sane");
	printf("\r\n");
#else
/*[tc_keystroke-test]*/
setbuf(stdout, NULL);
while (true) {
	c = tc_keystroke();
	putchar(c);
	if (c == 5) { /* ^E */
		ec_false( tc_restore() )
		printf("\nExiting...\n");
		exit(EXIT_SUCCESS);
	}
}
/*[]*/
#endif
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
