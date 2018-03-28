/**
 * Written by Tyler Montbriand.  Freeware.
 */
#ifdef _WIN32_WCE

#include <stdio.h>
#include "SDL_types.h"
#include <windows.h>
#include <notify.h>

static HANDLE hnotify=NULL;

static CE_NOTIFICATION_TRIGGER trigger=
	{sizeof(CE_NOTIFICATION_TRIGGER),CNT_TIME,0,L"",L"",{0},{0}};

static CE_USER_NOTIFICATION notify={0,NULL,NULL,NULL,0,0};

int stop_wakeup();

int schedule_wakeup(Sint32 ticks)
{
  union
  {
	FILETIME ftime;
	Uint64 itime;
  } utime;
  if(ticks < 0) return(0);

  if(hnotify != NULL)
  {
    if(stop_wakeup() < 0)
	  return(-1);
  }

  //GetSystemTime(&trigger.stStartTime);
  GetLocalTime(&trigger.stStartTime);
  SystemTimeToFileTime(&trigger.stStartTime,&utime.ftime);
  utime.itime += ticks*10000;
  FileTimeToSystemTime(&utime.ftime,&trigger.stStartTime);
  FileTimeToSystemTime(&utime.ftime,&trigger.stEndTime);
  hnotify=CeSetUserNotificationEx(NULL,&trigger,&notify);
  if(hnotify==NULL)
  {
	fprintf(stderr,"Couldn't set notification");
    return(-1);
  }

  fprintf(stderr,"Scheduled notification in %d seconds(%p)\n",
	  ticks/1000,hnotify);

  return(0);
}

int stop_wakeup()
{
  if(hnotify==NULL) return(0);

  fprintf(stderr,"Attempting to stop notification(%p)\n",hnotify);

  if(CeClearUserNotification(hnotify))
  {
	fprintf(stderr,"Notification stopped\n");
    hnotify=NULL;
	return(0);
  }

  fprintf(stderr,"Couldn't stop notification\n");

  return(-1);
}
#else
int schedule_wakeup(Sint32 ticks)
{
  return(-1);
}

int stop_wakeup()
{
  return(-1);
}
#endif
