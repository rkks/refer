/*
** kioctlname.c for  in /goinfre/kdump/darwin
** 
** Made by nicolas
** Login   <nico@epita.fr>
** 
** Started on  Tue Nov  1 18:18:08 2005 nicolas
** Last update Tue Nov  1 18:59:16 2005 nicolas
*/

#include <stdlib.h>
#include "kioctl.h"

const char	*ioctlname(register_t val)
{
  int		count;

  for (count = 0; gl_ioctlname[count].name; count++)
    if (gl_ioctlname[count].name == val)
      return (gl_ioctlname[count].str);
  return (NULL);
}
