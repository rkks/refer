/*
** console.c
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

/* TEMPORARY !!
 * Should be a real user service
 * Currently put this code here ...
 */

#include <console/console.h>

static unsigned scr_count = 0;
static unsigned char* const video = (unsigned char*) 0xB8000;

#define WIDTH	80
#define HEIGHT	25

void	console_clear(void)
{
  for (unsigned i = 0; i < (WIDTH*HEIGHT*2); i+=2)
    {
      *(video + i) = ' ';
      *(video + i + 1) = 0;
    }
}

static void	printchar(char c)
{
  if (scr_count >= (WIDTH*HEIGHT*2)) scr_count = 0;
  *(video + scr_count) = c;
  *(video + scr_count + 1) = 0x02;
  scr_count+=2;
}

void	console_write(const char* str)
{
  for (; *str; str++) printchar(*str);
}

void	console_write_line(const char* str)
{
  console_write(str);
  unsigned space_count = WIDTH - ((scr_count/2) % WIDTH);
  for (; space_count; space_count--)
    printchar(' ');
}

static void	printul(unsigned long ul)
{
  unsigned long lu = ul / 10;
  if (lu) printul(lu);
  lu = ul % 10;
  printchar(lu + '0');
}

void	console_write_nbr(unsigned long nbr)
{
  printul(nbr);
}

static void	printulhex(unsigned long addr)
{
  unsigned long lu = addr / 0x10;
  if (lu) printulhex(lu);
  lu = addr % 0x10;
  if (lu < 10)
    printchar(lu + '0');
  else
    printchar(lu % 10 + 'A');
}

void	console_write_addr(void* addr)
{
  console_write("0x");
  printulhex((unsigned long)addr);
}
