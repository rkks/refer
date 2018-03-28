/*
** ftrigger.c for  in /home/nico/lang/kernel/kld-filetrigger
** 
** Made by nicolas
*/

/* Headers
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/kernel.h>
#include <sys/kthread.h>
#include <sys/unistd.h>
#include "ftrigger.h"
#include "server.h"
#include "die.h"

/* Forward declarations
 */

/* Extern functions
 */
void	launch_ftrigger_server(void)
{
  server_t* singleton = get_server();
  if (!singleton) die("launch_ftrigger_server: server allocation failed"); /* TODO */
  if (singleton->init_with_port(singleton, 4243)) die("launch_ftrigger_server: init_with_port failed");
  singleton->loop(singleton);
}

void	terminate_ftrigger_server(void)
{
  server_t* singleton = get_server();
  if (!singleton) die("launch_ftrigger_server: server allocation failed"); /* TODO */
  singleton->destroy(singleton);
}

int	ftrigger_syscall_interface(int type, char* str_a, short s_a, unsigned ui_a, char* str_b)
{
  server_t* singleton = get_server();
  switch (type)
    {
    case SHARE:
      singleton->command_share(singleton, str_a);
      break;
    case REGISTER:
      singleton->command_register(singleton, str_a, s_a, ui_a, str_b);
      break;
    default:
      return -1;
    }
  return 0;
}
