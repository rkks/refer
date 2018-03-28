#include <stdio.h>
#include <sys/loadavg.h>
#include "ssp.h"

int main (void)
{
	double load_av [3];

	if (getloadavg (load_av, 3) == -1)
		err_msg ("getloadavg failed");

	printf ("Load averaged over 1 minute: %.2f\n", load_av [LOADAVG_1MIN]);
	printf ("Load averaged over 5 minutes: %.2f\n", load_av [LOADAVG_5MIN]);
	printf ("Load averaged over 15 minutes: %.2f\n", load_av [LOADAVG_15MIN]);

	return (0);
}
