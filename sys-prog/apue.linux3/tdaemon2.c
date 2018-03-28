#include	"ourhdr.h"

int
main(void)
{
	char	*ptr, buff[MAXLINE];

	daemon_init();

	close(0);
	close(1);
	close(2);

	ptr = getlogin();
	sprintf(buff, "login name: %s\n",
					(ptr == NULL) ? "(empty)" : ptr);
	write(3, buff, strlen(buff));
	exit(0);
}
