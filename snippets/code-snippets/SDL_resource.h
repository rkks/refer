/**
 * SDL_resource.h (c) Tyler Montbriand, 2005.  Simple, modular runtime 
 * resource-loading for SDL_image, SDL_ttf, and SDL_mixer.  Licensed
 * under the GNU LGPL.
 * http://www.gnu.org/copyleft/lesser.txt 
 */

#ifndef __SDL_RESOURCE_H__
#define __SDL_RESOURCE_H__

/**
 * \file SDL_resource.h
 * A modular way to gracefully load resources at runtime.  Given a list 
 * of source types and resource types, it loads them at runtime and 
 * gracefully handles errors.  So far it currently supports graphics and
 * truetype font types, memory and file sources.
 */

#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>

typedef enum restype_e
{
  RES_GRAPHIC,
  RES_FONT,
  RES_SOUND,
  RES_LAST,
  RES_INVALID=-1
} restype_e;

typedef enum srctype_e
{
  SRC_MEM,
  SRC_FILE,
  SRC_INVALID=-1
} srctype_e;

typedef struct restype
{
  const restype_e restype;
  const srctype_e srctype;

  union
  {
    struct
    {
      void *ptr;
      int size;
    } memory;
    const char *filename;
  } source;

  union
  {
	struct
	{
	  Uint8 usekey;
	  Uint8 r;
	  Uint8 g;
	  Uint8 b;
	} graphic;

    struct
    {
      int ptsize;
      int index;
    } font;


    Uint8 volume;
  } params;

  union
  {
    SDL_Surface *graphic;
    TTF_Font *font;
    Mix_Chunk *sound;
  } data;
  int mustload;
  int loaded;
} restype;

#define RES_FONT(resarray,index) ((resarray)[index].data.font)
#define RES_GRAPHIC(resarray,index) ((resarray)[index].data.graphic)
#define RES_SOUND(resarray,index) ((resarray)[index].data.sound)

#include <SDL/begin_code.h>
#ifdef __cplusplus
extern "C" {
#endif

/* Explicitly loads a specific resource if not loaded, even nonessential ones */
DECLSPEC int SDLCALL SDL_LoadResource(restype *resource, const char *relpath);

/* Explicitly free a resource, if loaded */
DECLSPEC int SDLCALL SDL_FreeResource(restype *resource);

/* Loads all nonessential resources in a NULL-terminated resource array */
DECLSPEC int SDLCALL SDL_LoadResources(restype resources[],
                                       const char *relpath);
/* Frees all loaded resources in a resource array */
DECLSPEC int SDLCALL SDL_FreeResources(restype resources[]);

/* Frees all non-musthave resources in a resource array */
DECLSPEC int SDLCALL SDL_FreeDeferredResources(restype resources[]);

#ifdef __cplusplus
}
#endif
#include <SDL/close_code.h>

#endif/*__SDL_RESOURCE_H__*/
