#include <sys/syscall.h>
#include <unistd.h>

int main()
{
  syscall(210, "toto", 5);
}
