/***************************************************************************
 *   SDL_rwhttp.h -- external function definitions for HTTP TCP/IP         *
 *     SDL_RWops streams                                                   *
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
#ifndef __SDL_RWHTTP_H__
#define __SDL_RWHTTP_H__

#include <SDL/SDL_rwops.h>

#include <SDL/begin_code.h>
#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

/**
 * Reads data from an HTTP file/page source.
 * @param auth optionally specifies simple authentication authrozation,
 * of the form user:pass.  Leave it NULL for no auth.
 * @param host is the server, like "burningsmell.org".  Omit the HTTP.
 * @param path is the file, like "/index.html".  / is needed.
 * @param port is the HTTP port.  Standard port is 80.
 */
DECLSPEC SDL_RWops * SDLCALL SDL_RWFromHTTP(const char *auth,
                                            const char *host,
                                            const char *path,
                                            Uint16 port);

#ifdef __cplusplus
}
#endif/*__cplusplus*/
#include <SDL/close_code.h>

#endif/*__SDL_RWHTTP_H__*/
