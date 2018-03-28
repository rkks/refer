/***************************************************************************
 *   TCP/IP client socket SDL_RWops stream implimentation                  *
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
#include <ctype.h>
#ifdef __APPLE__
#include "SDL_net.h"
#else
#include <SDL/SDL_net.h>
#endif
#include "SDL_rwsock.h"
#include "SDL_rwlib_internal.h"
#include "SDL_rwprintf.h"

#define PKGNAME SOCK

RWFUNC_SEEK_STUB(PKGNAME);
RWFUNC_READ(PKGNAME);
RWFUNC_WRITE(PKGNAME);
RWFUNC_CLOSE(PKGNAME);

SDL_RWops *SDL_RWFromSock(TCPsocket sock, int autoclose)
{
  SDL_RWops *rw=SDL_AllocRW();
  if(rw==NULL) return(NULL);

  RW_INITFUNC(rw,PKGNAME);
  RW_DATAPTR1(rw)=(void *)sock;
  RW_DATAINT(rw)=autoclose;
  return(rw);
}

SDL_RWops *SDL_RWFromHost(const char *host, Uint16 port)
{
  SDL_RWops *rw=SDL_AllocRW();
  IPaddress ip;
  TCPsocket sock;

  if(rw==NULL)
    return(NULL);

  if(SDLNet_ResolveHost(&ip,host,port)<0)
  {
    SDL_FreeRW(rw);
    return(NULL);
  }

  sock=SDLNet_TCP_Open(&ip);
  if(sock==NULL)
  {
    SDL_FreeRW(rw);
    return(NULL);  
  }

  RW_INITFUNC(rw,PKGNAME);
  RW_DATAPTR1(rw)=(void *)sock;
  RW_DATAINT(rw)=1;
  return(rw);
}

RWFUNC_READ(PKGNAME)
  /*(SDL_RWops *ctx, void *ptr, int size, int maxnum)*/
{
  return(SDLNet_TCP_Recv((TCPsocket)RW_DATAPTR1(ctx),ptr,size*maxnum));
}

RWFUNC_WRITE(PKGNAME)
  /*(SDL_RWops *ctx, const void *ptr, int size, int maxnum)*/
{ 
  return(SDLNet_TCP_Send((TCPsocket)RW_DATAPTR1(ctx),(void *)ptr,size*maxnum));
}

RWFUNC_CLOSE(PKGNAME)
  /*(SDL_RWops *ctx)*/
{
  if(ctx==NULL)
  {
    SDL_SetError("rw_sock_close:  Invalid RWops");
    return(-1);
  }
  else if(RW_TYPE(ctx)!=RW_TYPENUM(SOCK))
  {
    SDL_SetError("rw_sock_close:  RWops is the wrong type");
    return(-1);
  }

  if(RW_DATAINT(ctx))
    SDLNet_TCP_Close((TCPsocket)RW_DATAPTR1(ctx));

  SDL_FreeRW(ctx);
  return(0);
}

#ifdef RW_SOCK_TEST

#include <SDL/SDL_thread.h>
#include "SDL_rwfd.h"

int running=2;

int threadread(SDL_RWops *sock)
{
  while(running)
  {
    char buf[512];
    int size=SDL_RWread(sock,buf,1,512);
    if(size>0) SDL_RWwrite(rwout,buf,1,size);
    else break;
  }

  rwprintf(rwerr,"Read thread exiting\n");
  running--;
  return(1);
}

int threadwrite(SDL_RWops *sock)
{
  while(running>1)
  {
    char buf[512];
    int size=SDL_RWread(rwin,buf,1,512);
    if(size>0) SDL_RWwrite(sock,buf,1,size);
    else if(size<=0) break;
  }

  rwprintf(rwerr,"Write thread exiting\n");
  running--;
  return(1);
}

int main(int argc, char *argv[])
{
  SDL_RWops *s;
  SDL_Init(0);
  SDLNet_Init();

  s=SDL_RWFromHost("smtmuck.dyndns.org",8888);
  if(s==NULL)
  {
    rwprintf(rwerr,"Couldn't open socket\n");
    SDLNet_Quit();
    SDL_Quit();
    return(1);
  }

  SDL_CreateThread(threadwrite,s);
  SDL_CreateThread(threadread,s);

  while(running)
    SDL_Delay(100);

  SDLNet_Quit();
  SDL_Quit();
  return(0);
}
#endif/*RW_SOCK_TEST*/
