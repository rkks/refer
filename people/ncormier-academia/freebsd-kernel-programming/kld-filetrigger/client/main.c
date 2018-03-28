/*
** main.c<2> for  in /home/nico/lang/kernel/kld-filetrigger
** 
** Made by nicolas
*/

#include <sys/syscall.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

struct syscall_args
{
  int		type;
  char*		str_a;
  short		s_a;
  unsigned	ui_a;
  char*		str_b;
};

int usage(void)
{
  printf("./client share file\n");
  printf("./client register addr port id file\n");
  printf("./client timeout value\n");
  return 1;
}


int main(int argc, char** argv)
{
  int mode = 0;

  if (argc < 2)
    return usage();
  if (!strcmp(argv[1], "share"))
    mode = 1;
  else if (!strcmp(argv[1], "register"))
    mode = 2;
  else if (strcmp(argv[1], "register"))
    return usage();
  if (mode) /* Share */
    {
      if (argc < 3)
	return usage();
      struct syscall_args args;
      args.type = 0;
      args.str_a = argv[2];
      args.s_a = 0;
      args.ui_a = 0;
      args.str_b = NULL;
      syscall(210, &args);
    }
  else if (mode == 2) /* Register */
    {
      if (argc < 6)
	return usage();
      struct syscall_args args;
      args.type = 1;
      args.str_a = argv[2];
      args.s_a = atoi(argv[3]);
      args.ui_a = atoi(argv[4]);
      args.str_b = argv[5];
      syscall(210, &args);
    }
  else
    {
      if (argc < 3)
	return usage();
      struct syscall_args args;
      args.type = 2;
      args.str_a = NULL;
      args.s_a = atoi(argv[3]);
      args.ui_a = 0;
      args.str_b = NULL;
      syscall(210, &args);
    }
  return 0;
}
