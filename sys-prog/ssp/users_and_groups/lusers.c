#include <stdio.h>
#include <pwd.h>
#include "ssp.h"

int main (void)
{
	struct passwd *pwd;

	setpwent ();

	while ((pwd = getpwent ()) != NULL) {
		if (*pwd -> pw_gecos == '\0') {
			printf ("\"%s\" is %s (%ld, %ld)\n", pwd -> pw_name,
				pwd -> pw_name, (long) pwd -> pw_uid, (long) pwd -> pw_gid);
		}
		else {
			printf ("\"%s\" is %s (%ld, %ld)\n", pwd -> pw_gecos,
				pwd -> pw_name, (long) pwd -> pw_uid, (long) pwd -> pw_gid);
		}
	}

	endpwent ();

	return (0);
}
