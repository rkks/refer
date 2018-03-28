#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ssp.h"

static char *ltostr (long value, int base);

int main (void)
{
	long num;
	int base;

	num = 12345;
	base = 26;

	printf ("%ld is %s in base %d\n", num, ltostr (num, base), base);

	return (0);
}

static char *ltostr (long value, int base)
{
	char *nums = "0123456789abcdefghijklmnopqrstuvwxyz";
	static char buf [LINE_LEN];
	long ovalue;
	char *ptr;
	char tmp;
	int mod;
	int i;
	int n;

	if ((base < 2) || (base > 36))
		return (NULL);

	ovalue = value;
	ptr = buf;

	do {
		mod = labs (value % base);
		*ptr++ = nums [mod];
		value /= base;
	} while (value != 0);

	if (ovalue < 0)
		*ptr++ = '-';
	*ptr = '\0';

	n = strlen (buf) - 1;

	for (i = 0; i < (n / 2); i++) {
		tmp = buf [i];
		buf [i] = buf [n - i];
		buf [n - i] = tmp;
	}

	return (buf);
}
