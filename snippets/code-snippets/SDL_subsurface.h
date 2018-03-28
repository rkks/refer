/**
 * (C) Tyler Montbriand, 2005
 * Released under the GNU LGPL
 * http://burningsmell.org/ tsm@accesscomm.ca
 */
#ifndef __SDL_SUBSURFACE_H__
#define __SDL_SUBSURFACE_H__

#include "SDL_video.h"

#include "begin_code.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Returns a surface created from an area of another surface.  It uses 
 * the EXACT SAME PIXEL DATA as the original surface -- write to an area 
 * on the subsurface, and changes will be reflected in the parent.
 *
 * Creating a subsurface requires locking the parent surface, so it is 
 * not reccomended that one create subsurfaces for anything but software 
 * surfaces.
 */
DECLSPEC SDL_Surface * SDLCALL SDL_SubSurface(SDL_Surface *parent,
  const SDL_Rect *area);

#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif/*__SDL_SUBSURFACE_H__*/
