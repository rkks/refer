#include <stdio.h>
#include <shadow.h>
#include <errno.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	struct spwd *spwd;
	int i;

	for (i = 1; i < argc; i++) {
		if ((spwd = getspnam (argv [i])) == NULL)
			if (errno == 0)
				printf ("%s: No such user\n", argv [i]);
			else
				err_ret ("getspnam failed");
		else {
			printf ("User name: %s\n", spwd -> sp_namp);
			printf ("Encrypted password: %s\n", spwd -> sp_pwdp);
			printf ("Last change: %d\n", (int) spwd -> sp_lstchg);
			printf ("Min: %d\n", (int) spwd -> sp_min);
			printf ("Max: %d\n", (int) spwd -> sp_max);
			printf ("Warn: %d\n", (int) spwd -> sp_warn);
			printf ("Inact: %d\n", (int) spwd -> sp_inact);
			printf ("Expires: %d\n", (int) spwd -> sp_expire);
			printf ("Flags: %d\n", (int) spwd -> sp_flag);
			printf ("\n");
		}
	}

	return (0);
}
