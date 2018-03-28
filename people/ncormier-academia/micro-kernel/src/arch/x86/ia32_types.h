/*
** ia32_types.h
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#ifndef __ARCH_X86_IA32_TYPES_H__
# define __ARCH_X86_IA32_TYPES_H__


/* Boolean type
 */
typedef unsigned char	BOOL;
#define YES	1
#define NO	0

/* Address type
 * paddr_t for physical address
 * vaddr_t for virtual address
 */
typedef unsigned long	paddr_t, vaddr_t;

/* Offset type
 */
typedef unsigned long	offset_t;

/* Size type
 */
typedef unsigned long	size_t;

/* Register type
 */
typedef unsigned long	register_t;

/* Page size
 */
# define PAGESIZE	4096

/* Nil value
 */
# define NIL	((void*)0x0)

#endif /* __ARCH_X86_IA32_TYPES_H__ */
