#include <stdio.h>
#include <pwd.h>

int main (int argc, char **argv)
{
	struct passwd *pwd;
	int i;

	for (i = 1; i < argc; i++) {
		if ((pwd = getpwnam (argv [i])) == NULL)
			printf ("%s: No such user\n", argv [i]);
		else {
			printf ("User name: %s\n", pwd -> pw_name);
			printf ("User ID: %ld\n", (long) pwd -> pw_uid);
			printf ("Group ID: %ld\n", (long) pwd -> pw_gid);
			printf ("GECOS: %s\n", pwd -> pw_gecos);
			printf ("Home directory: %s\n", pwd -> pw_dir);
			printf ("Login shell: %s\n", pwd -> pw_shell);
			printf ("\n");
		}
	}

	return (0);
}
