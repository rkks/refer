#include <SDL/SDL_thread.h>
#include <SDL_rwfd.h>
#include <SDL_rwsock.h>

/*
SDL_sem *sem=NULL;

int rdt_func(SDL_RWops *rdsrc)
{
  char buf[512];
  int read;

  while((read=SDL_RWread(rdsrc,buf,1,512))>0)
  {
    if(SDL_RWwrite(rwout,buf,1,read)<read)
    {
      fprintf(stderr,"EOF on stdout");
      break;
    }
  }

  SDL_SemPost(sem);
}

int wrt_func(SDL_RWops *wrdst)
{
  char buf[512];
  int read;

  while((read=SDL_RWread(rwin,buf,1,512))>0)
  {
    if(SDL_RWwrite(wrdst,buf,1,read)<read)
    {
      fprintf(stderr,"EOF on socket");
      break;
    }
  }

  SDL_SemPost(sem);
//  SDL_RWclose(wrdst);
}*/

int main(int argc, char *argv[])
{
  char buf[512];
  int bytes;
  int total=0;

  SDL_RWops *sock;
//  SDL_Thread *rdt,*wrt;

  if(argc != 3)
  {
    fprintf(stderr,"syntax:  %s hostname port\n\n",argv[0]);
    return(1);
  }

  SDL_Init(0);
  SDLNet_Init();

//  sem=SDL_CreateSemaphore(0);

  sock=SDL_RWFromHost(argv[1],atoi(argv[2]));
  if(sock == NULL)
  {
    fprintf(stderr,"Could not open %s port %d\n",argv[1],atoi(argv[2]));
//    SDL_DestroySemaphore(sem);
    return(1);
  }

//  wrt=SDL_CreateThread(wrt_func,sock);
//  rdt=SDL_CreateThread(rdt_func,sock);
  while((bytes=SDL_RWread(rwin,buf,1,512))>0)
  {
    total+=bytes;
    if(SDL_RWwrite(sock,buf,1,bytes) < bytes)
    {
      fprintf(stderr,"Connection broken");
      break;
    }
  }

/*  SDL_SemWait(sem);
  SDL_SemWait(sem);

  SDL_WaitThread(rdt,NULL);
  SDL_WaitThread(wrt,NULL);*/

  SDLNet_Quit();
  SDL_Quit();
//  SDL_DestroySemaphore(sem);

  fprintf(stderr,"Wrote %d bytes\n",total);

  return(0);
}
