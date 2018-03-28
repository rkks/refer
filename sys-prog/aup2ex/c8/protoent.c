/*
	getprotoent example
	AUP2, Sec. 8.08.3 (not in book)

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
#include <netdb.h>

int main(void)
{
	struct protoent *p;
	int i;

	setprotoent(true);
	while ((p = getprotoent()) != NULL) {
		printf("%d %s", p->p_proto, p->p_name);
		for (i = 0; p->p_aliases[i] != NULL; i++)
			printf(" %s", p->p_aliases[i]);
		printf("\n");
	}
	endprotoent();
	exit(EXIT_SUCCESS);
}
