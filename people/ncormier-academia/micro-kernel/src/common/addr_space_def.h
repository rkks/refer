/*
** addr_space_def.h
**
** This file contains information about the organisation
** of the kernel virtual address space and the physical address
** space.
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
** 
** 
** 
** Exemple of IA32 kernel virtual address space description:
**
**
**	+------------------+ <- 0xFFF00000
**	|                  |
**	|   Kernel Stack   |
**	|                  |
**	\/\/\/\/\/\/\/\/\/\/	(512 Mo)
**	\/\/\/\/\/\/\/\/\/\/
**	|                  |
**	|   Kernel Heap    |
**	|                  |
**	+------------------+ <- 0xDFF00000
**	|                  |
**	| Kernel Pages     |	(256 Mo)
**	|      Collection  |
**	|                  |
**	+------------------+ <- 0xCFF00000
**	|                  |
**	|  Kernel Binary   |	(256 Mo)
**	|                  |
**	+------------------+ <- 0xBFF00000
**	|                  |
**	|      User        |	(~3 Go)
**	|                  |
**	+------------------+ <- 0x00000000
**
*/

#ifndef __COMMON_ADDR_SPACE_DEF_H__
# define __COMMON_ADDR_SPACE_DEF_H__


# include <arch/x86/ia32_addr_space_def.h>


#endif /* __COMMON_ADDR_SPACE_DEF_H__  */
