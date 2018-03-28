#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "ssp.h"

static void print_uids (void);

int main (void)
{
	print_uids ();

	printf ("Trying seteuid (1)...\n");
	if (seteuid (1) == -1)
		err_ret ("seteuid failed");
	print_uids ();

	printf ("Trying seteuid (2)...\n");
	if (seteuid (2) == -1)
		err_ret ("seteuid failed");
	print_uids ();

	printf ("Trying seteuid (0)...\n");
	if (seteuid (0) == -1)
		err_ret ("seteuid failed");
	print_uids ();

	printf ("Trying setuid (1)...\n");
	if (setuid (1) == -1)
		err_ret ("setuid failed");
	print_uids ();

	printf ("Trying seteuid (0)...\n");
	if (seteuid (0) == -1)
		err_ret ("seteuid failed");
	print_uids ();

	printf ("Trying setuid (0)...\n");
	if (setuid (0) == -1)
		err_ret ("setuid failed");
	print_uids ();

	return (0);
}

static void print_uids (void)
{
	printf ("UID = %ld, EUID = %ld\n", (long) getuid (), (long) geteuid ());
}
