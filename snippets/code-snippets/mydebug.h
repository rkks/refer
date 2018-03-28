/**
 * mydebug.h -- Written by Tyler Montbriand, 2005.
 * tsm@accesscomm.ca http://burningsmell.org/
 * Freeware.
 */

/* Prevent mydebug.h from being included more than once */
#ifndef __MYDEBUG_H__
#define __MYDEBUG_H__

/* Makes these functions work even if you include and use this in a .cpp 
file */
#ifdef __cplusplus
extern "C" {
#endif

/* Don't call directly, use DEBUG_PRINT(("something")) macros */
void _debug_print(const char *cmdstr, ...);
void _info_print(const char *cmdstr, ...);

#ifdef __cplusplus
}
#endif

/**
 * INFO_PRINT always calls _info_print, even if it might NOT actually 
print anything,
 * so use it sparingly, it'll bloat the final program.
 */
#define INFO_PRINT(X) _info_print X

/**
 * Will ONLY be called when DEBUG is defined, should magically disappear
 * otherwise.  Use all you want, will not bloat non-DEBUG builds.
 */
#ifdef DEBUG
#define DEBUG_PRINT(X) _debug_print X
#else
#define DEBUG_PRINT(X)
#endif

#endif/*__MYDEBUG_H__*/
