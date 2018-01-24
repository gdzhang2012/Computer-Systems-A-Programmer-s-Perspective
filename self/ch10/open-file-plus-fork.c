#include "csapp.h"
#include <sys/wait.h>

int main()
{
  int fd1, fd2;
  char c;

  fd1 = Open("foobar.txt", O_RDONLY, 0);
  fd2 = Open("foobar.txt", O_RDONLY, 0);
  if (fork() == 0)
  {
    printf("Child\n");
    Read(fd1, &c, 1);
    Read(fd2, &c, 1);
    printf("fd1:%d, c:%c\n", fd1, c);
    printf("fd2:%d, c:%c\n", fd2, c);
    Close(fd1);
    Close(fd2);
    exit(0);
  }

  wait(NULL);
  printf("\n");
 
  printf("Parent\n");
  Read(fd1, &c, 1);
  Read(fd2, &c, 1);
  printf("fd1:%d, c:%c\n", fd1, c);
  printf("fd2:%d, c:%c\n", fd2, c);
  Close(fd1);
  Close(fd2);
  exit(0);
}
