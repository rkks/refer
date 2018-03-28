#include <rctl.h>
#include "ssp.h"

extern int print_rctls (const char *name);
static int callback (const char *name, void *pvt);

int main (void)
{
	if (rctl_walk (callback, NULL) == -1)
		err_msg ("callback failed");

	return (0);
}

static int callback (const char *name, void *pvt)
{
	return (print_rctls (name));
}
