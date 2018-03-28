#include <stdarg.h>
#include <stdio.h>

void
error(const char *format, ...)
{
    va_list ap;
    extern char *programName;

    va_start(ap, format);

    fprintf(stderr, "%s: ", programName);
    vfprintf(stderr, format, ap);

    va_end(ap);
}
