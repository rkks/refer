/***************************************************************************
 *   SDL_rwring.h -- external function definitions for threadsafe          *
 *     ringbuffer SDL_rwops                                                *
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
 ***************************************************************************
 *      SDL_rwring -- A threadsafe ring-buffer using SDL and SDL_RWops     *
 *                                                                         *
 *   Creates a ring buffer usable via the standard SDL_RWops read, write,  *
 *   and close functions.  Seek, of course, has no meaning for a ring      *
 *   buffer.                                                               *
 *                                                                         *
 *   The object size you use for reads and writes must be equal to or a    *
 *   multiple of the blocksize chosen when the buffer was created.         *
 ***************************************************************************/

#ifndef __SDL_RWRING_H__
#define __SDL_RWRING_H__

#include <SDL/SDL_rwops.h>
#include <SDL/begin_code.h>

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

/**
 * Creates a new ring buffer of 'size' bytes, in 'blocksize' blocks.
 * For instance, a 1024-byte ring buffer with blocks of 256 bytes would
 * contain 4 blocks.
 *
 * Timeout sets the number of milliseconds a read  or write can be blocked
 * before it will fail.  A value of -1 disables timeouts.
 */
DECLSPEC SDL_RWops *SDLCALL SDL_RWringbuf(int size, int blocksize, int timeout);

/**
 * Returns 0 if there is data available to read, 1 if no data available,
 * or -1 on error.
 */
DECLSPEC int         SDLCALL SDL_RWringbuf_empty(SDL_RWops *rw);

#ifdef __cplusplus
}
#endif/*__cplusplus*/

#include <SDL/close_code.h>

#endif/*__SDL_RWRING_H__*/
