/*
	Byte order example
	AUP2, Sec. 8.01.4

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
#include <arpa/inet.h>

/*[pgm]*/
int main(void)
{
	uint16_t nhost = 0xD04C, nnetwork;
	unsigned char *p;

	p = (unsigned char *)&nhost;
	printf("%x %x\n", *p, *(p + 1));
	nnetwork = htons(nhost);
	p = (unsigned char *)&nnetwork;
	printf("%x %x\n", *p, *(p + 1));
	exit(EXIT_SUCCESS);
}

