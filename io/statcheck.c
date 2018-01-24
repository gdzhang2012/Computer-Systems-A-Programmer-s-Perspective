#include "csapp.h"

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("Usage: stat_sample.out <pathname>\n");

    return -1;
  } 

  struct stat stat;
  char *type, *readok;

  Stat(argv[1], &stat);
  if (S_ISREG(stat.st_mode))
  {
    type = "Regular"; 
  }
  else if (S_ISDIR(stat.st_mode))
  {
    type = "Directory";
  }
  else
  {
    type = "Others";
  }

  if (stat.st_mode & S_IRUSR)
  {
    readok = "Yes";
  }
  else
  {
    readok = "No";
  }

  printf("Type: %s\nRead: %s\n", type, readok);

  return 0;
}
