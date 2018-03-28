#include <sys/types.h>
#include <sys/stat.h>
#include "ssp.h"

int main (void)
{
	struct stat buf;

	if (stat ("foo", &buf) == -1)
		err_ret ("Stat failed");

	if (chmod ("foo", (buf.st_mode & ~(S_IWGRP | S_IWOTH)) | S_IXUSR) == -1)
		err_ret ("Chmod failed for foo");

	if (chmod ("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) == -1)
		err_ret ("Chmod failed for bar");

	return (0);
}
