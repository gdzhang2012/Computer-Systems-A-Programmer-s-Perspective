#include "csapp.h"

int main()
{
  int fd1, fd2;
  char c;

  fd1 = Open("foobar.txt", O_RDONLY, 0);
  fd2 = Open("foobar.txt", O_RDONLY, 0);
  Read(fd1, &c, 1);
  printf("c = %c\n", c);
  dup2(fd2, fd1);
  Read(fd1, &c, 1);
  printf("c = %c\n", c);

  Close(fd1);
  Close(fd2);

  exit(0);
}
