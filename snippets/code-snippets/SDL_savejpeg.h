/**
 * Copyright (C) 2004 Pluto, Inc., a Florida Corporation
 * www.plutohome.com
 *
 * Phone: +1 (877) 758-8648
 * This program is distributed according to the terms of the Pluto Public 
 * License, available at:
 * http://plutohome.com/index.php?section=public_license
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the Pluto 
 * Public License for more details.
 */

/**
 * Adapted from JpegWrapper.h by Tyler Montbriand, 2005
 * http://svn.plutohome.com/pluto/trunk/src/Orbiter/SDL/JpegWrapper.h
 */
#ifndef __SDL_SAVEJPEG_H__
#define __SDL_SAVEJPEG_H__

#include "begin_code.h"

#ifdef __cplusplus
extern "C" {
#endif

#define JPEG_QUALITY_MIN 0
#define JPEG_QUALITY_MAX 100
#define JPEG_QUALITY_DEFAULT 70
#define SDL_SaveJPEG(P,N,Q) SDL_SaveJPG(P,N,Q)

DECLSPEC int SDLCALL SDL_SaveJPG(SDL_Surface *pSurface,
  const char *sFileName,
  int iQuality);

#ifdef __cplusplus
extern "C" }
#endif

#include "close_code.h"

#endif/*__SDL_SAVEJPEG_H__*/
