#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <lastlog.h>
#include <time.h>
#include "ssp.h"

static struct lastlog *getlluid (uid_t uid);

int main (int argc, char **argv)
{
	struct lastlog *ll_entry;
	uid_t uid;
	int i;

	if (argc < 2)
		err_quit ("Usage: lastlog uid ...");

	for (i = 1; i < argc; i++) {
		uid = atol (argv [i]);
		if ((ll_entry = getlluid (uid)) == NULL)
			continue;

		if (ll_entry -> ll_time != 0)
			printf ("%-8ld  %-8s  %-16s  %s", uid, ll_entry -> ll_line,
				ll_entry -> ll_host, ctime (&ll_entry -> ll_time));
		else
			printf ("%-8ld  never logged in\n", (long) uid);
	}

	return (0);
}

static struct lastlog *getlluid (uid_t uid)
{
	static struct lastlog ll_entry;
	FILE *fp;

	if ((fp = fopen ("/var/adm/lastlog", "r")) == NULL)
		return (NULL);

	fseek (fp, uid * sizeof (struct lastlog), SEEK_SET);
	fread (&ll_entry, sizeof (struct lastlog), 1, fp);
	fclose (fp);

	return (&ll_entry);
}
