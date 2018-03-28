/**
 * Simple API for accessing config files.  It supports multiple config 
 * files and nested directories, creating directories as needed.
 *
 * If your application needs only one file, you can do something like
 *     SDL_OpenSaveFile("appname", "w")
 *
 * If your application needs several, you can use several like:
 *     SDL_OpenSaveFile("appname/keymaps", "w")
 *     SDL_OpenSaveFile("appname/state", "w");
 *     SDL_OpenSaveFile("appname/userdata", "w");
 *
 * The file modes work like stdio -- "w" overwrites and truncates,
 * "r" opens for read-only, and so forth.
 *
 * It returns an SDL_RWops structure.
 */

#ifndef __HOMEDIR_H__
#define __HOMEDIR_H__

#include <SDL/SDL_rwops.h>

DECLSPEC SDL_RWops * SDLCALL SDL_OpenSaveFile(const char *name,
	const char *mode);

#endif/*__HOMEDIR_H__*/
