/**
 * SDL_resource.c (c) Tyler Montbriand, 2005.  Simple, modular runtime
 * resource-loading for SDL_image, SDL_ttf, and SDL_mixer.  Licensed
 * under the GNU LGPL.
 * http://www.gnu.org/copyleft/lesser.txt
 */

#include <stdio.h>
#include <string.h>
#include <SDL/SDL_image.h>

#include <SDL/SDL_ttf.h>

#include "SDL_resource.h"

static void SDL_ResourceFreeFont(restype *resource)
  { if(resource->data.font!=NULL) TTF_CloseFont(resource->data.font); }

static void SDL_ResourceFreeGraphic(restype *resource)
  { if(resource->data.graphic!=NULL) SDL_FreeSurface(resource->data.graphic); }

static void SDL_ResourceFreeSound(restype *resource)
  { if(resource->data.sound!=NULL) Mix_FreeChunk(resource->data.sound); }

static int SDL_ResourceLoadSound(SDL_RWops *source, restype *resource)
{
  int retval=0;
  resource->data.sound=Mix_LoadWAV_RW(source,1);

  if(resource->data.sound == NULL)
    SDL_RWclose(source);
  else
    resource->data.sound->volume=resource->params.volume;

  return(retval);
}

static int SDL_ResourceLoadGraphic(SDL_RWops *source, restype *resource)
{
  SDL_Surface *screen;
  SDL_Surface *base;

  screen=SDL_GetVideoSurface();
  base=IMG_Load_RW(source,0);

  if(base==NULL)
  {
    SDL_RWclose(source);
    return(-1);
  }

  if(screen!=NULL)
  {
    SDL_Surface *cvt=SDL_DisplayFormat(base);
    if(cvt!=NULL)
    {
      SDL_FreeSurface(base);
      base=cvt;
    }
  }

  if(resource->params.graphic.usekey)
  {
    Uint32 key=SDL_MapRGB(base->format,
		resource->params.graphic.r,
		resource->params.graphic.g,
		resource->params.graphic.b);
	SDL_SetColorKey(base,SDL_SRCCOLORKEY,key);
  }

  SDL_RWclose(source);
  resource->data.graphic=base;
  return(0);
}

static int SDL_ResourceLoadFont(SDL_RWops *source, restype *resource)
{
  TTF_Font *font;

  font=TTF_OpenFontIndexRW(source,1, resource->params.font.ptsize,
                                     resource->params.font.index);

  if(font==NULL)
    return(-1);

  resource->data.font=font;
  return(0);
}

static int (*ResLoad[RES_LAST])(SDL_RWops *source, restype *resource) = 
  { SDL_ResourceLoadGraphic,
    SDL_ResourceLoadFont,
    SDL_ResourceLoadSound    };

static void (*ResFree[RES_LAST])(restype *resource) =
  { SDL_ResourceFreeGraphic,
    SDL_ResourceFreeFont,
    SDL_ResourceFreeSound };

static SDL_RWops *GetSource(restype *resource, const char *relpath);

int SDL_FreeResource(restype *resource)
{
  if((resource->restype < 0)||(resource->restype >=RES_LAST))
    return(-1);
  if(!resource->loaded) return(0);

  ResFree[resource->restype](resource);
  resource->loaded=0;
  return(0);
}

int SDL_FreeDeferredResources(restype resources[])
{
  int n,retval=0;

  for(n=0; resources[n].restype >= 0; n++)
  {
    if(resources[n].mustload) continue;

    if(SDL_FreeResource(resources+n)<0)
    {
      SDL_SetError("Couldn't free resource %d",n);
      retval--;
    }
  }

  return(retval);
}

int SDL_FreeResources(restype resources[])
{
  int retval=0;
  int n;
  for(n=0; resources[n].restype >= 0; n++)
    if(SDL_FreeResource(resources+n)<0)
    {
      SDL_SetError("Couldn't free resource %d",n);
      retval--;
    }

  return(retval);
}

int SDL_LoadResource(restype *resource, const char *relpath)
{
  SDL_RWops *src;

  if(resource->loaded) return(0);

  if((resource->restype<0)||(resource->restype >= RES_LAST))
    return(-1);
  src=GetSource(resource,relpath);
  if(src==NULL) return(-1);

  if(ResLoad[resource->restype](src,resource)<0)
    return(-1);

  resource->loaded=1;
  return(0);
}

int SDL_LoadResources(restype resources[], const char *relpath)
{
  int n;
  for(n=0; resources[n].restype >= 0; n++)
  {
    // Can defer loading to later for nonessentials
    if(!resources[n].mustload) continue;

    if(SDL_LoadResource(resources+n,relpath)<0)
    {
      n--;
      goto ERR_FREE;
    }
  }

  return(0);

ERR_FREE:
  while(n>=0)
  {
    ResFree[resources[n].restype](resources+n);
    n--;
  }
  return(-1);
}

static SDL_RWops *GetSource(restype *resource, const char *relpath)
{
  char buf[512];
  switch(resource->srctype)
  {
  int len;
  case SRC_MEM:
    return(SDL_RWFromConstMem(resource->source.memory.ptr,
                              resource->source.memory.size));

  case SRC_FILE:
    len=strlen(resource->source.filename);
    if(relpath!=NULL)
      len+=strlen(relpath);

    if(len>=512)
    {
      SDL_SetError("Filename too long");
      return(NULL);
    }

    buf[0]='\0';
    if(relpath!=NULL) strcpy(buf,relpath);
    strcat(buf,resource->source.filename);
    return(SDL_RWFromFile(buf,"rb"));

  default:
    return(NULL);
  }
}
