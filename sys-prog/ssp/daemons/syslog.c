#include <syslog.h>

int main (void)
{
	openlog ("ssp", LOG_CONS | LOG_PID, LOG_USER);

	syslog (LOG_ALERT, "Before changing log mask");
	syslog (LOG_ERR, "Before changing log mask");

	setlogmask (LOG_MASK (LOG_ERR));

	syslog (LOG_ALERT, "After changing log mask");
	syslog (LOG_ERR, "After changing log mask");

	closelog ();

	return (0);
}
