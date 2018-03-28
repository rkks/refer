/***************************************************************************
 *   SDL_rwtmp.h -- external function definition for temporary memory      *
 *     and file buffers for SDL_RWops streams                              *
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
#ifndef __SDL_RWTMP_H__
#define __SDL_RWTMP_H__

#include <SDL/SDL_rwops.h>

#include <SDL/begin_code.h>
#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Creates an SDL_RWops that serves as a temporary buffer */
DECLSPEC SDL_RWops * SDLCALL SDL_RWTmp();
/*! \brief Writes data in from another SDL_RWops without an external buffer */
DECLSPEC int         SDLCALL SDL_RWTmp_glob(SDL_RWops *buf,SDL_RWops *src,
                                            int size, int maxnum);
/*! \brief Like SDL_RWTmp(), except creates a temporary file */
DECLSPEC SDL_RWops * SDLCALL SDL_RWTmpFile();
#ifdef __cplusplus
}
#endif
#include <SDL/close_code.h>

#endif/*__SDL_RWTMP_H__*/
