#ifndef _C60_H_
#define _C60_H_

/* TODO extern C */

#include "utstring.h"

int c60_get_bucket(char *dest);
void *c60_client_init_fromfile(char *file, UT_string *err);
void c60_client_close(void *_c60);
int c60_send(void *c60, char *dest, char *msg, size_t len);

#endif // _C60_H_
