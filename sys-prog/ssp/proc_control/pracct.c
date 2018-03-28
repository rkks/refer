#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/acct.h>
#include <unistd.h>
#include <fcntl.h>
#include "ssp.h"

#define ACCT_FILE "/var/adm/pacct"

static unsigned long compt2ulong (comp_t num);

int main (void)
{
	int fd;
	int n;
	struct acct acct_data;

	if ((fd = open (ACCT_FILE, O_RDONLY)) == -1)
		err_msg ("Can't open %s", ACCT_FILE);

	while ((n = read (fd, &acct_data, sizeof (struct acct))) != 0) {
		if (n == -1)
			err_msg ("read failed");

		printf ("%-8s  %c%c  etime = %6ld, io = %8ld, stat = %3u\n",
			acct_data.ac_comm, (acct_data.ac_flag & AFORK) ? 'F': ' ',
			(acct_data.ac_flag & ASU) ? 'S' : ' ',
			compt2ulong (acct_data.ac_etime), compt2ulong (acct_data.ac_io),
			(unsigned char) acct_data.ac_stat);
	}

	return (0);
}

static unsigned long compt2ulong (comp_t num)
{
	unsigned long val;
	int exp;

	val = num & 017777;			/* 13-bit mantissa */
	exp = (num >> 13) & 7;		/* 3-bit exponent  */

printf ("val = %lu, exp = %d\n", val, exp);
	while (exp-- > 0)
		val *= 8;

	return (val);
}
