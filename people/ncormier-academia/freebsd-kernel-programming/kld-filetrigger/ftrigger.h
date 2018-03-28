/*
** ftrigger.h for  in /home/nico/lang/kernel/kld-filetrigger
** 
** Made by nicolas
*/

#ifndef __FTRIGGER_H__
# define __FTRIGGER_H__

/* Forward declaration
 */
# include "forward_typedefs.h"

/* Functions
 */
server_t*	server_singleton(void);
void		launch_ftrigger_server(void);
void		terminate_ftrigger_server(void);
int		ftrigger_syscall_interface(int type, char* str_a, short s_a, unsigned ui_a, char* str_b);

#endif /* __FTRIGGER_H__ */
