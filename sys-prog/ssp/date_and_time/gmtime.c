#include <stdio.h>
#include <time.h>

int main (void)
{
	struct tm *tp;
	time_t login;
	time_t logout;
	time_t session_length;

	login = 100000;
	logout = 200000;
	session_length = (time_t) difftime (logout, login);

	tp = gmtime (&session_length);

	printf ("Session length is %d days, %d hours, %d minutes, "
		"and %d seconds\n", tp -> tm_yday, tp -> tm_hour, tp -> tm_min,
		tp -> tm_sec);

	return (0);
}
