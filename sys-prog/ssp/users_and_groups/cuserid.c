#include <stdio.h>
#include <unistd.h>
#include "ssp.h"

int main (void)
{
	char *user;

	sleep (5);

	if ((user = cuserid (NULL)) == NULL)
		err_quit ("Not in /var/adm/utmp[x]");
	else
		printf ("Login name = %s\n", user);

	return (0);
}
