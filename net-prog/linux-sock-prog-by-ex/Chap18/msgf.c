/* msgf.c :
 *
 * Format and log a server message :
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <syslog.h>
#include <sys/types.h>

#include "quotes.h"

#define MAX_MSGBUF      256

/*
 * Report formatted message :
 */
void
msgf(char type,const char *format,...) {
    int er = errno;     /* Save errno */
    int mtype;
    va_list ap;
    char msgbuf[MAX_MSGBUF];

    switch ( type ) {
    case 'w' :
        mtype = LOG_WARNING;
        break;
    case 'e' :
        mtype = LOG_ERR;
        break;
    case 'i' :
    default  :
        mtype = LOG_INFO;
    }

    va_start(ap,format);
    vsnprintf(msgbuf,sizeof msgbuf-1,
        format,ap);
    va_end(ap);

    syslog(mtype,"%s",msgbuf);

    errno = er;         /* Restore errno */
}
