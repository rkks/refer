/*
** ktypes.h
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#ifndef __KERNEL_COMMON_TYPES_H__
# define __KERNEL_COMMON_TYPES_H__

# include <base/types.h>

/* Kernel function return status
 */
enum _kstatus {

    KS_FAILED = -1,
    KS_SUCCESS = 0,
    KS_UNKNOWN
};
typedef enum _kstatus	kstatus_t;

#endif /* __KERNEL_COMMON_TYPES_H__ */
