/**
 * Written by Tyler Montbriand.  Freeware.
 */
#ifndef __DIRSTACK_H__
#define __DIRSTACK_H__

#ifdef __cplusplus
extern "C" {
#endif

int pushd(const char *fname);
int popd();

#ifdef __cplusplus
}
#endif

#endif/*__DIRSTACK_H__*/
