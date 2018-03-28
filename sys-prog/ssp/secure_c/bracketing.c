#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

uid_t saved_euid;
gid_t saved_egid;

static void priv_off (void);
static void priv_on (void);
static void print_uids (void);

int main (void)
{
	priv_off ();
	print_uids ();

	priv_on ();
	print_uids ();

	priv_off ();
	print_uids ();

	return (0);
}

static void priv_off (void)
{
	static int first_time = 1;

	if (first_time) {
		saved_euid = geteuid ();
		saved_egid = getegid ();
		first_time = 0;
	}

	seteuid (getuid ());
	setegid (getgid ());
}

static void priv_on (void)
{
	seteuid (saved_euid);
	setegid (saved_egid);
}

static void print_uids (void)
{
	printf ("Real UID = %ld, GID = %ld\n", (long) getuid (), (long) getgid ());
	printf ("Effective UID = %ld, GID = %ld\n", (long) geteuid (),
		(long) getegid ());
}
