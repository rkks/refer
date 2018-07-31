/* misc.c :
 *
 * Misc. Code:
 */
#include "quotes.h"

/*
 * Return the basename of the pathname:
 */
char *
Basename(char *cmd) {
    char *bname = strrchr(cmd,'/');

    return bname ? bname + 1 : cmd;
}

/*
 * Skip over valid ticket chars :
 */
char *
strtick(char *str) {
    int ch;

    while ( (ch = *str)
        && ( isalpha(ch)
          || isdigit(ch)
          || ch == '_'
          || ch == '.'
           ) )
        ++str;

    return str;
}
