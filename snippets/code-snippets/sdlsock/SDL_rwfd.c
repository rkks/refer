/***************************************************************************
 *   SDL_rwfd.c -- SDL_RWops stream implimentation for UNIX file           *
 *     descriptors                                                         *
 *                                                                         *
 *   Copyright (C) 2004 by Tyler Montbriand                                *
 *   tsm@accesscomm.ca                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#define PKGNAME FD

#include <sys/types.h>

#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <SDL/SDL_error.h>
#include "SDL_rwfd.h"
#include "SDL_rwlib_internal.h"

#define RWLIB_NUM_FD 0xdead0001
#define STDIN_FD    0
#define STDOUT_FD   1
#define STDERR_FD   2

RWFUNC_SEEK(PKGNAME);
RWFUNC_READ(PKGNAME);
RWFUNC_WRITE(PKGNAME);
RWFUNC_CLOSE(PKGNAME);

static int sdl_FD_close_static(SDL_RWops *c);

static SDL_RWops rw_FD_stdin={ RWFNAME(PKGNAME,seek)
                             , RWFNAME(PKGNAME,read)
                             , RWFNAME(PKGNAME,write)
                             , sdl_FD_close_static
                             , RWLIB_NUM_FD
                             , {0,(FILE *)STDIN_FD} };

static SDL_RWops rw_FD_stdout={RWFNAME(PKGNAME,seek)
                             , RWFNAME(PKGNAME,read)
                             , RWFNAME(PKGNAME,write)
                             , sdl_FD_close_static
                             , RWLIB_NUM_FD
                             , {0,(FILE *)STDOUT_FD} };

static SDL_RWops rw_FD_stderr={RWFNAME(PKGNAME,seek)
                             , RWFNAME(PKGNAME,read)
                             , RWFNAME(PKGNAME,write)
                             , sdl_FD_close_static
                             , RWLIB_NUM_FD
                             , {0,(FILE *)STDERR_FD} };

SDL_RWops *rwin =&rw_FD_stdin, 
          *rwout=&rw_FD_stdout,
          *rwerr=&rw_FD_stderr;

SDL_RWops *SDL_RWFromFD(int fd, int autoclose)
{
  SDL_RWops *rw;

  if(fd<0)
  {
    SDL_SetError("SDL_RWFromFD:  Invalid file # %d",fd);
    return(NULL);
  }

  rw=SDL_AllocRW();
  if(rw==NULL)
  {
    SDL_SetError("Out of memory");
    return(NULL);
  }
  memset(rw,0,sizeof(SDL_RWops));

  RW_INITFUNC(rw,PKGNAME);
  RW_DATAINT(rw)=autoclose;
  RW_DATAPTR1(rw)=(void *)(long int)fd;

//  rw->seek=RWNAME(PKGNAME,seek);
//  rw->read=RWNAME(PKGNAME,read);
//  rw->write=RWNAME(PKGNAME),write);
//  rw->close=sdl_fd_close;
//  rw->type=RWLIB_NUM_FD;
//  rw->hidden.stdio.autoclose=autoclose;
//  rw->hidden.stdio.fp=(FILE *)(long int)fd;

  return(rw);
}

RWFUNC_SEEK(PKGNAME)
  /*(SDL_RWops *ctx, int offset, int whence)*/
{
  return(lseek((int)(long int)RW_DATAPTR1(ctx),offset,whence));
}

RWFUNC_READ(PKGNAME)
  /*(SDL_RWops *ctx, void *ptr, int size, int maxnum)*/
{
  int bytes=read((int)(long int)RW_DATAPTR1(ctx),ptr,size*maxnum);
  if(bytes%size) // Didn't manage to read all the records, realign
  {
    int off=-(bytes%size);
    lseek((int)(long int)RW_DATAPTR1(ctx),-(bytes%size),SEEK_CUR);
  }

  if(bytes>=0) return(bytes/size);
  else         return(-1);
}

RWFUNC_WRITE(PKGNAME)
  /*(SDL_RWops *ctx, const void *ptr, int size, int maxnum)*/
{
  int bytes=write((int)(long int)RW_DATAPTR1(ctx),ptr,size*maxnum);
  if(bytes%size) // Didn't manage to read all the records, realign
  {
    int off=-(bytes%size);
    lseek((int)(long int)RW_DATAPTR1(ctx),-(bytes%size),SEEK_CUR);
  }

  if(bytes>=0) return(bytes/size);
  else         return(-1);
}

RWFUNC_CLOSE(PKGNAME)
  /*(SDL_RWops *c)*/
{
  int retval=0;
  if(ctx==NULL)
  {
    SDL_SetError("sdl_fd_close: NULL context");
    return(-1);
  }
  else if(RW_TYPE(ctx)!=RW_TYPENUM(PKGNAME))
  {
    SDL_SetError("sdl_fd_close: context is not RWLIB_NUM_FD");
    return(-1);
  }

  if(RW_DATAINT(ctx))
    retval=close((int)(long int)RW_DATAPTR1(ctx));

  SDL_FreeRW(ctx);
  return(retval);  
}

static int sdl_FD_close_static(SDL_RWops *c)
{
  SDL_SetError("close is undefined for static sdl_fd constructs");
  return(-1);
}

#ifdef RW_FD_TEST
#include "SDL_rwprintf.h"
int main(int argc, char *argv[])
{
  char buf[80]={'\0'};
  int bytes;
  rwprintf(rwerr,"Hello World %d\n",42);
  bytes=SDL_RWread(rwin,buf,1,79);
  if(bytes>0) buf[bytes]='\0';
  rwprintf(rwout,"Read %d bytes: %s\n",bytes,buf);
  
  return(0);
}
#endif/*RW_FD_TEST*/
