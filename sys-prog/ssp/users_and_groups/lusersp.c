#include <stdio.h>
#include <shadow.h>
#include <errno.h>
#include "ssp.h"

int main (void)
{
	struct spwd *spwd;

	setspent ();

	while ((spwd = getspent ()) != NULL)
		printf ("%s: %s\n", spwd -> sp_namp, spwd -> sp_pwdp);

	endspent ();

	return (0);
}
