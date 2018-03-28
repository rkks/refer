#include	<sys/types.h>
#include	<shadow.h>
#include	"ourhdr.h"

int
main(void)		/* SVR4 version */
{
	struct spwd	*ptr;

	if ( (ptr = getspnam("stevens")) == NULL)
		err_sys("getspnam error");

	printf("sp_pwdp = %s\n",
				ptr->sp_pwdp == NULL || ptr->sp_pwdp[0] == 0 ?
				"(null)" : ptr->sp_pwdp);
	exit(0);
}
