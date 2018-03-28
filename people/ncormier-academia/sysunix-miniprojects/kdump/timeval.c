/*
** timeval.c for  in /home/nico/lang/c/kdump
** 
** Made by nicolas
** Login   <nico@epita.fr>
** 
** Started on  Fri Oct 28 22:00:58 2005 nicolas
** Last update Fri Oct 28 22:01:22 2005 nicolas
*/

#include <sys/time.h>

void     timeval_diff(struct timeval *start, struct timeval *end, struct timeval *diff)
{
  diff->tv_sec = end->tv_sec - start->tv_sec;
  if (end->tv_usec < start->tv_usec)
    diff->tv_usec = 1000000 + end->tv_usec - start->tv_usec, --diff->tv_sec;
  else
    diff->tv_usec = end->tv_usec - start->tv_usec;
}

void     timeval_set(struct timeval *t, long sec, long usec)
{
  t->tv_usec = usec;
  t->tv_sec = sec;
}
