/* misc.c :
 *
 * Misc. Functions:
 */
#include "common.h"

/*
 *
 * This function reports the error to
 * the log file and calls exit(1).
 */
void
bail(const char *on_what) {

    if ( errno != 0 )
        fprintf(stderr,"%s: ",strerror(errno));
    fprintf(stderr,"%s\n",on_what);

    exit(1);
}
