/**
 * ehash.h, (c) Tyler Montbriand, 2005
 * Licensed under the GNU LGPL
 * http://www.gnu.org/copyleft/lesser.txt
 */
#ifndef __EHASH_H__
#define __EHASH_H__

#include "SDL.h"

/**
 * Preprocessor macros that let you use various complex SDL events 
 * without nesting multiple switch statements.
 * Instead of:

    switch(event.type)
     {
     case SDL_QUIT:
       fprintf(stderr,"SDL_QUIT recieved\n");
       exit(1);

     case SDL_KEYDOWN:
       switch(event.key.keysym.sym)
       {
       case SDLK_F1:
         if(event.key.keysym.mod&KMOD_CTRL)
           fprintf(stderr,"You hi CTRL-F1\n");
         else
           fprintf(stderr,"You hit F1\n");
         break;
       default:
         break;
       }
     }

 * you can do

    switch(EventHash(&event))
    {
    case SDL_QUIT:
      fprintf(stderr,"SDL_QUIT recieved\n");
      exit(1);

    case SDL_KEYDOWN_H(SDLK_F1,0):
      fprintf(stderr,"You hit F1\n");
      break;

    case SDL_KEYDOWN_H(SDLK_F1,KMOD_CTRL):
      fprintf(stderr,"You hit CTRL-F1\n");
      break;

    default:
      break;
    }

 * Note that these hashes are highly specific.  CTRL-F1 and F1 are
 * different values, mousemotion with no buttons and dragging are 
 * different values, etc.  This is useful in many cases but can suprise
 * you if you're not watching for it.
 */

// Returns this value if it cannot hash the given event
#define EVENT_INVALID 0

// This lets us do hashing at compile-time
#define CONST_HASH(a,b,c,d)  (((a)<<0)|((b)<<8)|((c)<<16)|((d)<<24))

// Takes a button index and mask, like SDL_MOUSEDOWN_H(SDL_BUTTON_LEFT,0)
#define SDL_MOUSEDOWN_H(btn,mask) CONST_HASH(SDL_MOUSEBUTTONDOWN,btn,mask,0)

// Takes a button index and mask, like SDL_MOUSEUP_H(SDL_BUTTON_RIGHT,0)
#define SDL_MOUSEUP_H(btn,mask)     CONST_HASH(SDL_MOUSEBUTTONUP,btn,mask,0)

// Takes a keysym and a keymod, like SDL_KEYDOWN_H(SDLK_ESCAPE,KMOD_CTRL)
// Does NOT differentiate between left and right control, alt, shift, or 
// meta keys, so use KMOD_CTRL, KMOD_SHIFT, KMOD_ALT, or KMOD_META
// and not their left/right equivalents.
#define SDL_KEYDOWN_H(key,mod) CONST_HASH(SDL_KEYDOWN,        mod,key,0)
// Like above, but for key-up events
#define SDL_KEYUP_H(key,mod)   CONST_HASH(SDL_KEYUP,          mod,key,0)
// Takes a button mask, like SDL_MOUSEMOVE_H(SDL_BUTTON_LMASK)
#define SDL_MOUSEMOVE_H(st)    CONST_HASH(SDL_MOUSEMOTION,     st,  0,0)

#ifdef __cplusplus
extern "C" {
#endif

#include "begin_code.h"
DECLSPEC Uint32 SDLCALL EventHash(SDL_Event *event);
#include "close_code.h"

#ifdef __cplusplus
}
#endif

#endif/*__EHASH_H__*/
