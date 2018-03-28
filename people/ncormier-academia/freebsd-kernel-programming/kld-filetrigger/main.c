/*
** main.c for  in /home/nico/lang/kernel/kld-filetrigger
** 
** Made by nicolas
*/

#include "server.h"
#include "elem.h"
#include "die.h"

int main()
{
#ifdef MASTER
  server_t* master_serv = get_server();
  if (!master_serv)
    exit(1); /* TODO */
  master_serv->init_with_port(master_serv, 4242);
  master_serv->command_share(master_serv, "/goinfre/test/master/toto");
  master_serv->command_share(master_serv, "/goinfre/test/master/");
  master_serv->loop(master_serv);
#endif
#ifdef SLAVE
  server_t* slave_serv = get_server();
  if (!slave_serv)
    exit(1); /* TODO */
  slave_serv->init_with_port(slave_serv, 4243);
  slave_serv->command_register(slave_serv, "127.0.0.1", 4242, 0, "/goinfre/test/slave");
  slave_serv->loop(slave_serv);
#endif
}
