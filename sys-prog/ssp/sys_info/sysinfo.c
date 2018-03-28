#include <stdio.h>
#include <sys/utsname.h>
#include <sys/systeminfo.h>
#include "ssp.h"

#define psysinfo(name) print_sysinfo (#name, name)

static void print_sysinfo (const char *name, int command);

int main (void)
{
	struct utsname utsname;

	if (uname (&utsname) == -1)
		err_msg ("uname failed");
	printf ("Info from uname:\n");
	printf ("  sysname: %s\n", utsname.sysname);
	printf ("  nodename: %s\n", utsname.nodename);
	printf ("  release: %s\n", utsname.release);
	printf ("  version: %s\n", utsname.version);
	printf ("  machine: %s\n\n", utsname.machine);

	printf ("Info from sysinfo\n");
	psysinfo (SI_SYSNAME);
	psysinfo (SI_HOSTNAME);
	psysinfo (SI_RELEASE);
	psysinfo (SI_VERSION);
	psysinfo (SI_MACHINE);
	psysinfo (SI_ARCHITECTURE);
#ifdef SI_ISALIST
	psysinfo (SI_ISALIST);
#endif
	psysinfo (SI_PLATFORM);
	psysinfo (SI_HW_PROVIDER);
	psysinfo (SI_HW_SERIAL);
	psysinfo (SI_SRPC_DOMAIN);
#ifdef SI_DHCP_CACHE
	psysinfo (SI_DHCP_CACHE);
#endif

	return (0);
}

static void print_sysinfo (const char *name, int command)
{
	char buf [257];

	if (sysinfo (command, buf, 257) == -1)
		err_msg ("sysinfo (%s) failed", name);
	printf ("  %s: %s\n", name, buf);
}
