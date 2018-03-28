/*
** sem.c for  in /home/nico/work/ipc
** 
** Made by nicolas
** Login   <nico@epita.fr>
** 
** Started on  Sun Jun  5 23:53:28 2005 nicolas
** Last update Sun Jun  5 23:53:32 2005 nicolas
*/

#include <stdarg.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int		get_or_create_sem(int key, int nbinit)
{
  int		sem;
  union semun	semun;
  u_short	array[1];

  if ((sem = semget(key, 1, 0)) == -1)
    {
      if ((sem = semget(key, 1, IPC_CREAT | 0600)) == -1)
	{
	  perror("semget");
	  exit(1);
	}
      array[0] = nbinit;
      semun.array = array;
      if (semctl(sem, 0, SETALL, semun) < 0)
	{
	  perror("semctl");
	  exit(1);
	}
    }
  return (sem);
}


void		sem_p(int sem)
{
  struct sembuf	sembuf;

  sembuf.sem_num = 0;
  sembuf.sem_op  = -1;
  sembuf.sem_flg = SEM_UNDO;
  if (semop(sem, &sembuf, 1) < 0)
    {
      perror("semop");
      exit(1);
    }
}

void		sem_v(int sem)
{
  struct sembuf	sembuf;

  sembuf.sem_num = 0;
  sembuf.sem_op = 1;
  sembuf.sem_flg = SEM_UNDO;
  if (semop (sem, &sembuf, 1) < 0)
    {
      perror("semop");
      exit(1);
    }
}

int             fdprintf(int fd, char *format, ...)
{
  va_list       ap;
  char          buf[BUFSIZ];

  va_start(ap, format);
  if (vsprintf(buf, format, ap) == -1)
    return (-1);
  if (write(fd, buf, strlen(buf)) == -1)
    return (-1);
  va_end(ap);
  return (strlen(buf));
}


int	main(int argc, char **argv)
{
  int	key;
  int	sem;
  int	fd;
    
  if (argc != 2)
    {
      perror("fichier cle ...\n");
      exit(1);
    }
  if ((key = ftok(argv[1], 0)) == -1)
    {
      perror("ftok");
      exit(1);
    }
  sem = get_or_create_sem(key, 1);
  while (1)
    {
      sem_p(sem);
      if ((fd = open("./log", O_CREAT|O_WRONLY|O_APPEND, 0600)) == -1)
	{
	  perror("open");
	  exit(1);
	}
      if (fdprintf(fd, "%d\n", getpid()) == -1)
	{
	  perror("open");
	  exit(1);
	}
      close(fd);
      sleep(1);
      sem_v(sem);
    }
  return (0);
}
