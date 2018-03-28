/*
** _stack.c
** Stack setup for IA32
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#include <base/assert.h>


static void	_move_kernel_stack(void* buf, size_t buf_size)
{
  /* Query kernel stack information
   */
  char* kern_stack = (char*) buf;
  size_t kern_stack_size = buf_size;

  /* Get registers value
   */
  char* old_ebp, * old_esp;
  char* new_ebp, * new_esp;
  __asm__ volatile("mov %%ebp, %0": "=r" (old_ebp)); 
  __asm__ volatile("mov %%esp, %0": "=r" (old_esp)); 
  new_esp = new_ebp = kern_stack + kern_stack_size;

  /* Copy current stack
   *
   * Only works with gcc -fomit-frame-pointer
   * The good way is to backtrace the stack and each ebp
   * with the new ebp.
   */
  ASSERT(kern_stack_size > (unsigned)(old_ebp - old_esp), "Bootstrap stack is too big");
  while (old_ebp > old_esp) *--new_esp = *--old_ebp;
  __asm__ volatile("mov %0, %%eax": : "m" (new_ebp));
  __asm__ volatile("mov %eax, %ebp");
  __asm__ volatile("mov %0, %%eax": : "m" (new_esp));
  __asm__ volatile("mov %eax, %esp");
}
