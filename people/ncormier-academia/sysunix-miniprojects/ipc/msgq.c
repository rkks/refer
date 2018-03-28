/*
** msgq.c for  in /goinfre/prog_linux/29
** 
** Made by nicolas
** Login   <nico@epita.fr>
** 
** Started on  Sun Jun  5 23:56:26 2005 nicolas
** Last update Mon Jun  6 00:19:05 2005 nicolas
*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>

typedef struct 
{
  long	mtype;    /* message type */
  char	mtext[256]; /* body of message */
}	msgp_t;

int		main(int argc, char **argv)
{
  int		key;
  int		msg;
  msgp_t	message;
    
  if (argc < 4)
    {
      perror("fichier_cle {read|write} type [msg]...\n");
      exit(1);
    }
  if ((key = ftok(argv[1], 0)) == -1)
    {
      perror("ftok");
      exit(1);
    }
  if ((msg = msgget(key, IPC_CREAT | 0600)) == -1)
    {
      perror("msgget");
      exit(1);
    }
  if (strcmp(argv[2], "write") == 0)
    {
      if (argc != 5)
	{
	  perror("fichier_cle {read|write} type [msg]...\n");
	  exit(1);
	}
      message.mtype = atoi(argv[3]);
      memcpy(message.mtext, argv[4], strlen(argv[4]) + 1);
      if (msgsnd(msg, (void *) &message, 256, 0) < 0)
	{
	  perror("msgsnd");
	  exit(1);
	}
    }
  if (strcmp(argv[2], "read") == 0)
    {
      if (msgrcv (msg, (void *) &message, 256, atoi(argv[3]), 0) >= 0)
	fprintf(stdout, "(%ld) %s \n", message.mtype, message.mtext);
      else
	{
	  perror("msgsnd");
	  exit(1);
	}
    }
}
