/****************************************/
/*                                      */
/*      monitor.h                       */
/*                                      */
/*      Alan Dix                        */
/*                                      */
/*      command interpretter to         */
/*      monitor conference server       */
/*                                      */
/*      11th January 1990               */
/*                                      */
/****************************************/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "monitor.h"

static perform_help()
{
    struct mon_tab_struct *mt;
    mess("available commands\n");
    mess("  %6s  -  print this text\n","help");
    for ( mt = monitor_tab; mt->name; mt++ ) {
        mess("  %6s  -  %s\n",mt->name,mt->help);
        }
}

perform_line(line)
    char *line;
{
    char command[256];
    char *bp=command, *lp=line;
    struct mon_tab_struct *mt;
    while ( *lp && !isspace(*lp) ) *bp++ = *lp++;
    *bp = '\0';
    while(isspace(*lp)) lp++;
    if (strcmp(line,"help") == 0 ) {
        perform_help();
        return 0;
        }
    for ( mt = monitor_tab; mt->name; mt++ ) {
        if ( strcmp(command,mt->name) != 0 ) continue;
        return (*(mt->f))( mt->id, mt->name, lp );
        }
    mess("%s not recognised\n",command);
    perform_help();
    return 0;
}

