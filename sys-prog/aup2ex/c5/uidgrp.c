/*
	Display real and effective user and group IDs
	AUP2, Sec. 5.11

	Copyright 2003 by Marc J. Rochkind. All rights reserved.
	May be copied only for purposes and under conditions described
	on the Web page www.basepath.com/aup/copyright.htm.

	The Example Files are provided "as is," without any warranty;
	without even the implied warranty of merchantability or fitness
	for a particular purpose. The author and his publisher are not
	responsible for any damages, direct or incidental, resulting
	from the use or non-use of these Example Files.

	The Example Files may contain defects, and some contain deliberate
	coding mistakes that were included for educational reasons.
	You are responsible for determining if and how the Example Files
	are to be used.

*/
#include "defs.h"
#include <pwd.h>
#include <grp.h>

/*[uidgrp]*/
int main(void)
{
	uid_t uid;
	gid_t gid;
	struct passwd *pwd;
	struct group *grp;

	uid = getuid();
	ec_null( pwd = getpwuid(uid) )
	printf("Real user = %ld (%s)\n", (long)uid, pwd->pw_name);

	uid = geteuid();
	ec_null( pwd = getpwuid(uid) )
	printf("Effective user = %ld (%s)\n", (long)uid, pwd->pw_name);

	gid = getgid();
	ec_null( grp = getgrgid(gid) )
	printf("Real group = %ld (%s)\n", (long)gid, grp->gr_name);

	gid = getegid();
	ec_null( grp = getgrgid(gid) )
	printf("Effective group = %ld (%s)\n", (long)gid, grp->gr_name);

	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
/*[]*/
