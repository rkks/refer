/****************************************/
/*                                      */
/*      line_by_line.c                  */
/*                                      */
/*      Alan Dix                        */
/*                                      */
/*      line input protocol             */
/*                                      */
/*      8th January 1990                */
/*                                      */
/****************************************/

#include <stdio.h>

#include "mess.h"
#include "fd.h"
#include "inform.h"
#include "line_by_line.h"

#ifndef BUFFSIZE
#define BUFFSIZE 100
#endif

struct d_s {
   int       active;
   line_fun  line;
   eof_fun   eof;
   id_type   id;
   char      line_buff[BUFFSIZE];
   int       len;
   };

struct d_s table[MAX_FD];

static perform_line(fd,tp)
    int   fd;
    struct d_s *tp;
{
    tp->line_buff[tp->len] = '\0';
    tp->line(fd,tp->id,tp->line_buff);
    tp->len = 0;
}

static handle_line_by_line(fd,id)
    int   fd;
    void *id;
{
    char buff[256];
    char *bp;
    struct d_s *tp = &table[fd];
    int n = read(fd,buff,BUFFSIZE-1);
    if ( n==0 ) {
        eof_line_by_line(fd);
        return 0;
        }
    buff[n] = '\0';
    for ( bp = buff; *bp; bp++ ) {
        if ( *bp == '\n' ) {
            perform_line(fd,tp);
            continue;
            }
        tp->line_buff[tp->len++] = *bp;
        if ( tp->len >= BUFFSIZE-1 ) {
            perform_line(fd,tp);
            }
        }
    return 0;
}

int    inform_line_by_line(fd,line,eof,id)
    int         fd;
    line_fun    line;
    eof_fun     eof;
    id_type     id;
{
    if ( ! OK_FD(fd) ) return -1;
    if ( table[fd].active ) {
        inform_input(fd,NULL,NULL);
        }
    if ( inform_input(fd,handle_line_by_line,NULL) != 0 ) return -1;
    table[fd].line   = line;
    table[fd].eof    = eof;
    table[fd].id     = id;
    table[fd].len    = 0;
    table[fd].active = 1;
    return 0;
}

int eof_line_by_line(fd)
    int fd;
{
    struct d_s *tp;
    if ( ! OK_FD(fd) ) return -1;
    tp = &table[fd];
    if ( ! tp->active ) return -1;
    inform_input(fd,NULL,NULL);
    if ( tp->len > 0 ) perform_line(fd,tp);
    tp->eof(fd,tp->id);
    tp->active = 0;
    return 0;
}

int cancel_line_by_line(fd)
    int fd;
{
    if ( ! OK_FD(fd) ) return -1;
    if ( table[fd].active ) {
        inform_input(fd,NULL,NULL);
        }
    table[fd].active = 0;
    return 0;
}
