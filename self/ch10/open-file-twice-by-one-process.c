#include "csapp.h"

int main()
{
  int fd1, fd2;
  char c;

  fd1 = Open("foobar.txt", O_RDONLY, 0);
  printf("fd1 = %d\n", fd1);

  fd2 = Open("foobar.txt", O_RDONLY, 0);
  printf("fd2 = %d\n", fd2);

  Read(fd1, &c, 1);
  printf("c = %c\n", c);
  Close(fd1);

  Read(fd2, &c, 1);
  printf("c = %c\n", c);
  Close(fd2);

  exit(0);
}
