#include "csapp.h"

int main(void)
{
  char buf[128] = "";

  while (rio_readn(STDIN_FILENO, buf, sizeof(buf) != 0))
  {
    rio_writen(STDOUT_FILENO, buf, sizeof(buf));
  }

  exit(0);
}
