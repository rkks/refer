/* Written by Tyler Montbriand.  Freeware. */
#ifndef __WCE_WAKEUP_H__
#define __WCE_WAKEUP_H__

#include "SDL_types.h"

#ifdef __cplusplus
extern "C" {
#endif

int schedule_wakeup(Sint32 ticks);
int stop_wakeup();

#ifdef __cplusplus
}
#endif

#endif/*__WCE_WAKEUP_H__*/
