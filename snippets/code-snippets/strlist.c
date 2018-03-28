#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strlist.h"

#define STRLIST_MINSIZE	16
#define STRLIST_FREE(X)	(((X)->size)-((X)->pos))

strlist *strlist_create(void)
{
	strlist *s=malloc(sizeof(strlist)+(sizeof(char *)*STRLIST_MINSIZE));
	s->pos=0;
	s->size=STRLIST_MINSIZE;
	return(s);
}

strlist *strlist_resize(strlist *s)
{
	strlist *n=realloc(s, sizeof(strlist) +
		(sizeof(char *)*(s->size<<1)));

	if(n == NULL)
		return(NULL);

	n->size<<=1;

	return(n);
}

void strlist_free(strlist *s)
{
	int n;
	for(n=0; n<s->pos; n++)
		free(s->list[n]);
	free(s);
}

strlist *strlist_append(strlist *s, const char *str)
{
	if(STRLIST_FREE(s) <= 0)
	{
		strlist *n=strlist_resize(s);
		if(n == NULL)
			return(NULL);
		s=n;
	}

	s->list[s->pos++]=strdup(str);
	return(s);
}
