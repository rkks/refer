/****************************************/
/*                                      */
/*      mess.c                          */
/*                                      */
/*      Alan Dix                        */
/*                                      */
/*      print to tty and transcript     */
/*                                      */
/*      8th January 1990                */
/*                                      */
/****************************************/

#include  <stdio.h>
#include <varargs.h>
#include "mess.h"

FILE       *fp = NULL;

void  init_mess(transcript)
    char *transcript;
{
    fp = fopen(transcript,"w");
    if ( fp == NULL ) perror("opening transcript");
}

void  mess(fmt,va_alist)
    char *fmt;
    va_dcl
{
    va_list args;
    va_start(args);
/*    fmt = va_arg(args, char*); */
    vfprintf(stderr,fmt,args);
    fflush(stderr);
    if ( fp ) {
        vfprintf(fp,fmt,args);
        fflush(fp);
        }
    va_end();
}
