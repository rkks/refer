/*
** main.c for  in /home/nico/lang/kernel/kld-filetrigger
** 
** Made by nicolas
*/

#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"
#include "elem.h"
#include "die.h"
#include "user/userlib.h"


void* server_th(void* pv)
{
  server_t* serv = get_server();
  serv->loop(serv);
}

int main()
{
#ifdef MASTER
  server_t* master_serv = get_server();
  if (!master_serv)
    exit(1); /* TODO */
  master_serv->init_with_port(master_serv, 4242);
  pthread_t pth;
  pthread_create(&pth, NULL, (void*(*)(void*))server_th, NULL);
  sleep(1);
  {
    void* data = netmalloc(1024);
    memcpy(data, "TOTO", sizeof("TOTO"));
    netdetach(data);
  }
  sleep(1);
  {
    void* data = netattach(0);
    memcpy(data + 4, "TATA", sizeof("TATA"));
    netdetach(data);
  }
  while (1) sleep(1);
#endif
#ifdef SLAVE
  server_t* slave_serv = get_server();
  if (!slave_serv)
    exit(1); /* TODO */
  slave_serv->init_with_port(slave_serv, 4243);
  slave_serv->loop(slave_serv);
#endif
}
