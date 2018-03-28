#include <stdio.h>
#include <grp.h>

int main (int argc, char **argv)
{
	struct group *grp;
	char *member;
	int i;

	for (i = 1; i < argc; i++) {
		if ((grp = getgrnam (argv [i])) == NULL)
			printf ("%s: No such user\n", argv [i]);
		else {
			printf ("Group name: %s\n", grp -> gr_name);
			printf ("Password: \"%s\"\n", grp -> gr_passwd);
			printf ("Group ID: %ld\n", (long) grp -> gr_gid);
			printf ("Members:");
			while ((member = *grp -> gr_mem++) != NULL)
				printf (" %s", member);
			printf ("\n");
		}
	}

	return (0);
}
