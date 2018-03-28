#include "defs.h"
#include <sys/utsname.h>

/*[uname]*/
int main(void)
{
	struct utsname info;
	
	ec_neg1( uname(&info) )
	printf("sysname = %s\n", info.sysname);
	printf("nodename = %s\n", info.nodename);
	printf("release = %s\n", info.release);
	printf("version = %s\n", info.version);
	printf("machine = %s\n", info.machine);
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
/*[]*/
