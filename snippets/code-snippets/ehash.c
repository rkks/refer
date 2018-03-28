/**
 * ehash.c, (c) Tyler Montbriand, 2005
 * Licensed under the GNU LGPL
 * http://www.gnu.org/copyleft/lesser.txt
 */

#include "SDL.h"
#include "ehash.h"

//#define CONST_HASH(a,b,c,d)  (((a)<<0)|((b)<<8)|((c)<<16)|((d)<<24))
//#define SDL_MOUSEDOWN_H(btn)   CONST_HASH(SDL_MOUSEBUTTONDOWN,btn,  0,0)
//#define SDL_MOUSEUP_H(btn)     CONST_HASH(SDL_MOUSEBUTTONUP,  btn,  0,0)
//#define SDL_KEYDOWN_H(key,mod) CONST_HASH(SDL_KEYDOWN,        mod,key,0)
//#define SDL_KEYUP_H(key,mod)   CONST_HASH(SDL_KEYUP,          mod,key,0)
//#define SDL_MOUSEMOVE_H(st)    CONST_HASH(SDL_MOUSEMOTION,     st,  0,0)

#define MASK_BUTTON(x) (~SDL_BUTTON(x))
#define MOUSESTATE_EXCEPT(button) (SDL_GetMouseState(NULL,NULL)&MASK_BUTTON(button))

Uint32 EventHash(SDL_Event *event)
{
  int n;
  static const int kmod_flags[]={KMOD_CTRL,KMOD_SHIFT,KMOD_ALT,KMOD_META,0};
  switch(event->type)
  {
  case SDL_QUIT:
    return(SDL_QUIT);
  case SDL_MOUSEBUTTONDOWN:
    return(SDL_MOUSEDOWN_H(event->button.button,
                           MOUSESTATE_EXCEPT(event->button.button)));
  case SDL_MOUSEBUTTONUP:
    return(SDL_MOUSEUP_H(event->button.button,
                         MOUSESTATE_EXCEPT(event->button.button)));
  case SDL_KEYDOWN:
    for(n=0; kmod_flags[n]; n++)
      if(event->key.keysym.mod&kmod_flags[n]) event->key.keysym.mod|=kmod_flags[n];

    return(SDL_KEYDOWN_H(event->key.keysym.sym,event->key.keysym.mod));
  case SDL_KEYUP:
    return(SDL_KEYUP_H(event->key.keysym.sym,event->key.keysym.mod));
  case SDL_MOUSEMOTION:
    return(SDL_MOUSEMOVE_H(event->motion.state));
  default:
    return(0);
  }
}
