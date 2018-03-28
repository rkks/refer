/*
** _ram.c
** Invalid implementation
**
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

static void	_detect_ram(paddr_t* o_base, paddr_t* o_top)
{
  /* Steps TODO to detect the amout of ram available:
   *  @ Int 0x15, eax = 0xE820 
   *  @ Int 0x15, ax = 0xE881 
   *  @ Int 0x15, ax = 0xE801 
   *  @ Int 0x15, ah = 0xC7 
   *  @ Int 0x15, ah = 0x88 
   *  @ CMOS locations 0x17 and 0x18 
   *  @ Manual probing
   */

  /* Update setup results
   */
  *o_base= 0x00100000;
  *o_top = 0x00F00000;
}
