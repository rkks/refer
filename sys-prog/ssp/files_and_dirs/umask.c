#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ssp.h"

#define MASK (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

int main (void)
{
	umask (0);
	if (creat ("foo", MASK) == -1)
		err_ret ("Creation of foo failed");

	umask (S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if (creat ("bar", MASK) == -1)
		err_ret ("Creation of bar failed");

	return (0);
}
