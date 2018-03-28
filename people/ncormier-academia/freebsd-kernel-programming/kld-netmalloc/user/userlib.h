/*
** userlib.h for  in /home/nico/lang/kernel/kld-netmalloc/user
** 
** Made by nicolas
*/

/* User netmalloc functions
 */
void*	netmalloc(unsigned size);
void	netdetach(void* data);
void*	netattach(unsigned id);
void	netfree(void* data);

