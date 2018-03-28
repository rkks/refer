/** Simple per-user config file manager. */

// Linux config files:  Under $HOME/.appname/etc
#if defined(unix)||defined(__unix__)||defined(_POSIX_SOURCE)
#include <sys/stat.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL/SDL.h>

#include "strlist.h"

#define DIR_SEP_STR	"/"
#define CREATE_DIR(path) ( mkdir(path,0700)==0)

static int TEST_DIR(const char *path)
{
	struct stat s;
	if(stat(path,&s)!=0)    return(0);
	if(!S_ISDIR(s.st_mode)) return(0);
	return(1);
}

static int GETSAVENAME(char base[], const char *name)
{
	int n;
	strlist *slist=strlist_create();
	char buf[512];
	const char *home;

	if(name == NULL)
		return(-1);
	if(name[0] == '\0')
		return(-1);

	home=SDL_getenv("HOME");
	if(home == NULL)
		return(-1);

	strcpy(buf, name);
	strcpy(base, home);
	home=strtok(buf, DIR_SEP_STR);
	while(home != NULL)
	{
		slist=strlist_append(slist, home);
		home=strtok(NULL, DIR_SEP_STR);
	}

	for(n=0; n<(slist->pos-1); n++)
	{
//		fprintf(stderr, "Adding dir %s", slist->list[n]);
		strcat(base, DIR_SEP_STR);
		if(n == 0)
			strcat(base, ".");

		strcat(base, slist->list[n]);
//		fprintf(stderr," = %s\n", base);
		CREATE_DIR(base);
		if(!TEST_DIR(base))
			return(-1);
	}

	strcat(base, DIR_SEP_STR);
	strcat(base, slist->list[slist->pos-1]);
	strlist_free(slist);

	return(0);
}

// Windows config files:  Under $APPDATA/appname/etc
#elif defined(WIN32)

#define DIR_SEP_STR	"\\"
#define WIN32_LEAN_AND_MEAN
#define INVALID_FILE_ATTRIBUTES 0xFFFFFFFF

#include <windows.h>

#define CREATE_DIR(path) (CreateDirectory(path,0L)!=0)
static int TEST_DIR(const char *path)
{
	int result=GetFileAttributes(path);
	if(result == INVALID_FILE_ATTRIBUTES) return(0);
	return(result & FILE_ATTRIBUTE_DIRECTORY);
}

static int GETSAVENAME(char base[], const char *name)
{
	int n;
	strlist *slist=strlist_create();
	char buf[512];
	const char *home;

	if(name == NULL)
		return(-1);
	if(name[0] == '\0')
		return(-1);

	home=SDL_getenv("APPDATA");
	if(home == NULL)
		return(-1);

	strcpy(buf, name);
	strcpy(base, home);
	home=strtok(buf, DIR_SEP_STR);
	while(home != NULL)
	{
		slist=strlist_append(slist, home);
		home=strtok(NULL, DIR_SEP_STR);
	}

	for(n=0; n<(slist->pos-1); n++)
	{
//		fprintf(stderr, "Adding dir %s", slist->list[n]);
		strcat(base, DIR_SEP_STR);
//		if(n == 0)
//			strcat(base, ".");

		strcat(base, slist->list[n]);
//		fprintf(stderr," = %s\n", base);
		CREATE_DIR(base);
		if(!TEST_DIR(base))
			return(-1);
	}

	strcat(base, DIR_SEP_STR);
	strcat(base, slist->list[slist->pos-1]);
	strlist_free(slist);

	return(0);
}

#else
#error "Add GETSAVENAME, CREATE_DIR, and TEST_DIR for your platform"
#endif

SDL_RWops *SDL_OpenSaveFile(const char *name, const char *mode)
{
	char base[512]={0};

	if(GETSAVENAME(base, name) < 0)
		return(NULL);

//	fprintf(stderr, "%s ==> %s\n", name, base);
	return(SDL_RWFromFile(base, mode));
}
