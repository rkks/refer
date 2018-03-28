#ifndef __STRLIST_H__
#define __STRLIST_H__

typedef struct strlist
{
	int size;
	int pos;
	char *list[0];
} strlist;

#ifdef __cplusplus
extern "C" {
#endif

strlist *strlist_create(void);
strlist *strlist_resize(strlist *s);
void strlist_free(strlist *s);
strlist *strlist_append(strlist *s, const char *str);

#ifdef __cplusplus
}
#endif

#endif/*__STRLIST_H__*/
