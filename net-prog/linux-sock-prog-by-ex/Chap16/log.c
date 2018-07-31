/* log.c
 *
 * Logging Functions:
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

static FILE *logf = NULL;      /* Log File */

/*
 * Open log file for append:
 *
 * RETURNS:
 *  0   Success
 *  -1  Failed.
 */
int
log_open(const char *pathname) {
    
    logf = fopen(pathname,"a");
    return logf ? 0 : -1;
}

/*
 * Log information to a file:
 */
void
log(const char *format,...) {
    va_list ap;

    if ( !logf )
        return;        /* No log file open */

    fprintf(logf,"[PID %ld] ",(long)getpid());

    va_start(ap,format);
    vfprintf(logf,format,ap);
    va_end(ap);
    fflush(logf);
}

/*
 * Close the log file :
 */
void
log_close(void) {

    if ( logf )
        fclose(logf);
    logf = NULL;
}

/*
 * This function reports the error to
 * the log file and calls exit(1).
 */
void
bail(const char *on_what) {

    if ( logf ) {          /* Is log open? */
        if ( errno )             /* Error? */
            log("%s: ",strerror(errno));
        log("%s\n",on_what);    /* Log msg */
        log_close();
    }
    exit(1);
}
