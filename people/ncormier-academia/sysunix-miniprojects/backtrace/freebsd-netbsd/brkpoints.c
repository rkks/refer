/*
** brkpoints.c for  in /home/nico/lang/c/backtrace
**
** Made by nicolas
** Login   <nico@epita.fr>
**
** Started on  Mon Jan 16 13:59:48 2006 nicolas
** Last update Sat Jan 21 17:21:52 2006 nicolas cormier
*/

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>

#include "backtrace.h"

static caddr_t*	brkpoints = NULL;
static char*	humanbrk[256];
static int	len = 0;
static int	hlen = 0;

void	free_brkpoints(void)
{
  if (brkpoints)
    {
      free(brkpoints);
      brkpoints = NULL;
    }
}

static unsigned int	pow16(int pow)
{
  unsigned int		ret;

  for (ret = 1; pow; pow--)
    ret *= 16;
  return (ret);
}

unsigned int	axtoi(char *hex_str)
{
  unsigned int	ret = 0, pow = 0;
  char*		end, * tmp;

  if (!strncmp(hex_str, "0x", 2))
    hex_str += 2;
  end = hex_str;
  for (tmp = hex_str; *tmp; tmp++)
    end = tmp;
  for (tmp = end; tmp >= hex_str; tmp--, pow++)
    {
      if (*tmp <= 'f' && *tmp >= 'a')
	ret += (*tmp - 'a' + 10) * pow16(pow);
      else if (*tmp >= '0' && *tmp <= '9')
	ret += (*tmp - '0') * pow16(pow);
      else
	errexit("backtrace: bad arg");
    }
  return (ret);
}

void	add_rbrkpoints(caddr_t addr)
{
  if (!brkpoints)
    brkpoints = malloc(2 * sizeof(caddr_t*));
  else
    brkpoints = realloc(brkpoints, (len + 1) * sizeof(char*));
  if (!brkpoints)
    errexit("backtrace: malloc/realloc(), %s", strerror(errno));
  brkpoints[len] = addr;
  /* printf("Add brkp: 0x%x\n", addr); */
  brkpoints[++len] = NULL;
}

/*
** Virer les 40 axatoi
*/
void	add_brkpoints(char* brkp)
{
  if (!(*brkp >= '0' && *brkp <= '9'))
    {
      humanbrk[hlen++] = brkp;
      humanbrk[hlen] = NULL;
      return;
    }
  if (!brkpoints)
    brkpoints = malloc(2 * sizeof(caddr_t*));
  else
    brkpoints = realloc(brkpoints, (len + 1) * sizeof(char*));
  if (!brkpoints)
    errexit("backtrace: malloc/realloc(), %s", strerror(errno));
  brkpoints[len] = (caddr_t) axtoi(brkp);
  printf("Add brkp: 0x%x\n", axtoi(brkp));
  brkpoints[++len] = NULL;
}

void	del_brkpoints(int idx)
{
  for (; brkpoints[idx]; idx++)
    brkpoints[idx] = brkpoints[idx + 1];
}

char**	get_humanbrkps(void)
{
  if (hlen == 0)
    return (NULL);
  return (humanbrk);
}

caddr_t*	get_bkrpoints(void)
{
  return (brkpoints);
}

void		print_brks(void)
{
  caddr_t*	ptr;

  for (ptr = get_bkrpoints(); *ptr; ptr++)
    printf("0x%x, %d\n", (unsigned int) *ptr, (int) *ptr);
}
