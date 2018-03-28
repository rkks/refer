#include	<sys/types.h>
#include	<signal.h>
#include	<unistd.h>

int
raise(int signo)
{
	return( kill(getpid(), signo) );
}
