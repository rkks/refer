#include <stdio.h>
#include <grp.h>
#include "ssp.h"

int main (void)
{
	struct group *grp;

	setgrent ();

	while ((grp = getgrent ()) != NULL)
		printf ("%s is group %ld\n", grp -> gr_name, (long) grp -> gr_gid);

	endgrent ();

	return (0);
}
