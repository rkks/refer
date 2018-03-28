/***************************************************************************
 *   SDL_rwfd.h -- external function definitions for UNIX                  *
 *     file-descriptor SDL_RWops streams                                   *
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
#ifndef __SDL_RWFD_H__
#define __SDL_RWFD_H__

#include <SDL/SDL_rwops.h>
#include <stdarg.h>

#include <SDL/begin_code.h>
#ifdef __cplusplus
extern "C" {
#endif

/*!\brief SDL_RWops structures for standard input, output, and error */
extern DECLSPEC SDL_RWops *rwin,*rwout,*rwerr;

/*! \brief Creates an SDL_RWops structure from file descriptor. */
DECLSPEC SDL_RWops * SDLCALL SDL_RWFromFD(int fd, int autoclose);

#ifdef __cplusplus
}
#endif
#include <SDL/close_code.h>

#endif/*__SDL_RWFD_H__*/
