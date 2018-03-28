#include	<sys/types.h>
#include	<pwd.h>
#include	"ourhdr.h"

int
main(void)		/* 44BSD version */
{
	struct passwd	*ptr;

	if ( (ptr = getpwnam("stevens")) == NULL)
		err_sys("getpwnam error");

	printf("pw_passwd = %s\n",
				ptr->pw_passwd == NULL || ptr->pw_passwd[0] == 0 ?
				"(null)" : ptr->pw_passwd);
	exit(0);
}
