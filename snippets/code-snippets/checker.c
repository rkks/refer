/**
 * checker.c, (c) Tyler Montbriand, 2005
 * Licensed under the GNU LGPL
 * http://www.gnu.org/copyleft/lesser.txt
 *
 * An efficient way for drawing colored checkerboxes on SDL surfaces.
 * It draws one pattern of 4 checkered squares, and keeps doubling it
 * in height and width until the area is filled.
 */
#include <stdio.h>
#include "SDL.h"

static const SDL_Color dgrey={ 98,105, 98,SDL_ALPHA_OPAQUE};
static const SDL_Color lgrey={156,153,156,SDL_ALPHA_OPAQUE};

void checker_fill(SDL_Surface *s, const SDL_Rect *area, int size,
  SDL_Color cs1, SDL_Color cs2)
{
  SDL_Rect oldclip;
  Uint32 c1=SDL_MapRGB(s->format,cs1.r,cs1.g,cs1.b),
         c2=SDL_MapRGB(s->format,cs2.r,cs2.g,cs2.b);

  SDL_Rect r={area->x,area->y,size*2,size*2};

  SDL_GetClipRect(s,&oldclip);
  SDL_SetClipRect(s,area);

  {
    // Draw first checkerbox
    SDL_FillRect(s,&r,c1);
    r.x=area->x+size;
    r.w=size;
    r.h=size;
    SDL_FillRect(s,&r,c2);
    r.x=area->x;
    r.y=area->y+size;
    SDL_FillRect(s,&r,c2);
    r.x=area->x;
    r.y=area->y;
    r.w=size*2;
    r.h=size*2;
  }

  while((r.w < area->w) || (r.h < area->h))
  {
    SDL_Rect r2=r;
    if(r.w < area->w)
    {
      // Double checkboxes horizontally
      r2.x+=r2.w;
      SDL_BlitSurface(s,&r,s,&r2);
      r.w*=2;
    }
    else
    {
      // Double checkboxes vertically
      r2.y+=r2.h;
      SDL_BlitSurface(s,&r,s,&r2);
      r.h*=2;
    }
  }

  SDL_SetClipRect(s,&oldclip);
}

int main(int argc, char *argv[])
{
  SDL_Rect area={20,20,320-40,240-40};
  SDL_Surface *screen;
  if(SDL_Init(SDL_INIT_VIDEO)<0)
  {
    fprintf(stderr,"Couldn't init SDL: %s\n",SDL_GetError());
    return(1);
  }

  screen=SDL_SetVideoMode(320,240,32,SDL_ANYFORMAT);
  if(screen==NULL)
  {
    fprintf(stderr,"Couldn't init video: %s\n",SDL_GetError());
    SDL_Quit();
    return(1);
  }
  // Fill screen with black
  SDL_FillRect(screen,NULL,0);
  // Fill part of screen with checker pattern
  checker_fill(screen,&area,8,lgrey,dgrey);
  // Update screen
  SDL_Flip(screen);
  // Give time to see it
  SDL_Delay(1000);
  // All done
  SDL_Quit();
  return(0);  
}
