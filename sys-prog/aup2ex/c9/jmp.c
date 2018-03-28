/*
	longjmp example
	AUP2, Sec. 9.06

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
#include <setjmp.h>
/*[pgm]*/
static jmp_buf loc_info;

static void fcn2(void)
{
	printf("In fcn2\n");
	longjmp(loc_info, 1234);
	printf("Leaving fcn2\n");
}

static void fcn1(void)
{
	printf("In fcn1\n");
	fcn2();
	printf("Leaving fcn1\n");
}

int main(void)
{
	int rtn;

	rtn = setjmp(loc_info);
	printf("setjmp returned %d\n", rtn);
	if (rtn == 0)
		fcn1();
	printf("Exiting\n");
	exit(EXIT_SUCCESS);
}
/*[]*/
