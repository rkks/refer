#include <stdio.h>
#include <utmpx.h>
#include <pwd.h>
#include <time.h>
#include "ssp.h"

int main (void)
{
	struct utmpx *utmpx;
	struct passwd *pwd;
	char name [17];

	printf ("Login     Name              Line      Time             Host\n");
	printf ("===========================================================\n");

	setutxent ();

	while ((utmpx = getutxent ()) != NULL) {
		if (utmpx -> ut_type != USER_PROCESS)
			continue;

		if ((pwd = getpwnam (utmpx -> ut_user)) != NULL)
			snprintf (name, 17, "%s", pwd -> pw_gecos);
		else
			snprintf (name, 17, "#%ld", (long) pwd -> pw_uid);

		printf ("%-8s  %-16s  %-8s  %15.15s", utmpx -> ut_user, name,
			utmpx -> ut_line, ctime (&utmpx -> ut_tv.tv_sec) + 4);
		if (utmpx -> ut_syslen > 0)
			printf ("  %s", utmpx -> ut_host);

		printf ("\n");
	}

	endutxent ();

	return (0);
}
