/*
** console.h
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

/* TEMPORARY !!
 * Should be a real user service
 * Currently put this code here ...
 */

#ifndef __SERVICES_CONSOLE_CONSOLE_H__
# define __SERVICES_CONSOLE_CONSOLE_H__

void	console_clear(void);
void	console_write(const char* str);
void	console_write_line(const char* str);
void	console_write_nbr(unsigned long nbr);
void	console_write_addr(void* addr);

#endif /* __SERVICES_CONSOLE_CONSOLE_H__ */
