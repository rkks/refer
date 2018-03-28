#include <fcntl.h>
#include <errno.h>

int main(int argc, char** argv)
{
  int read_mode = 0;

  if (argc < 2)
    return 1;
  if (!strcmp(argv[1], "read"))
    read_mode = 1;
  else if (strcmp(argv[1], "write"))
    return 1;
  if (read_mode)
    {
      int fd = open("/dev/mysio0", O_RDONLY);
      if (fd == -1)
	return 1;
      char buf[256];
      bzero((void*)buf, sizeof(buf));
      if (read(fd, buf, sizeof(buf)) != -1)
	printf("[%s]\n", buf);
    }
  else
    {
      if (argc < 3)
	return 1;
      int fd = open("/dev/mysio1", O_WRONLY);
      if (fd == -1)
	return 1;
      printf("%d\n", write(fd, argv[2], strlen(argv[2])));
    }
  printf("errno: %d\n", errno);
  return 0;
}
