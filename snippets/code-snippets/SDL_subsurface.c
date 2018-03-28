/**
 * SDL_subsurface.c
 * (C) Tyler Montbriand, 2005
 * Released under the GNU LGPL
 * http://burningsmell.org/ tsm@accesscomm.ca
 */
#include "SDL_subsurface.h"
SDL_Surface *SDL_SubSurface(SDL_Surface *sin, const SDL_Rect *area)
{
  Uint8 *pdata=NULL;
  SDL_Surface *sout=NULL;
  if((area->x < 0)||(area->y < 0))
    return(NULL);
  if(((area->x + area->w) > sin->w) ||
     ((area->y + area->h) > sin->h) )
    return(NULL);
  if(SDL_MUSTLOCK(sin))
    if(SDL_LockSurface(sin)<0)
      return(NULL);

  pdata=sin->pixels;
  pdata+=(area->y * sin->pitch)+(area->x * sin->format->BytesPerPixel);

  return(SDL_CreateRGBSurfaceFrom(pdata,
    area->w,area->h,  
    sin->format->BitsPerPixel,
    sin->pitch,
    sin->format->Rmask,sin->format->Gmask,
    sin->format->Bmask,sin->format->Amask));
}
