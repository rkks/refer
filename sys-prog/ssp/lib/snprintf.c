#include <stdio.h>
#include <stdarg.h>
#include "ssp.h"

int snprintf (char *buf, size_t n, const char *fmt, ...)
{
	int len;
	va_list ap;

	va_start (ap, fmt);
	len = vsprintf (buf, fmt, ap);
	va_end (ap);

	if (len >= n)
		err_quit ("snprintf: \"%s\" caused a buffer overflow", fmt);

	return (len);
}
