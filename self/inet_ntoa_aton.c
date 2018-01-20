#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
  const char *ip_string = NULL;
  struct in_addr ip_struct;
  int i = 0;
  char *p = NULL;
  unsigned host_num = 0; 

  if (argc != 2)
  {
    printf("Usage: inet_ntoa_aton <ip str>\n");

    return -1;
  }

  ip_string = argv[1];
  inet_aton(ip_string, &ip_struct);

  p = (char *)(&(ip_struct.s_addr));
  for (i = 0; i < sizeof(unsigned); i++)
  {
    printf("[%d]:%02x	", i, *(p++));
  }
  printf("\n");
  printf("ip network order(big-endian): 0x%x\n", ip_struct.s_addr);
  printf("\n");

  host_num = ntohl(ip_struct.s_addr);
  p = (char *)(&host_num);
  for (i = 0; i < sizeof(unsigned); i++)
  {
    printf("[%d]:%02x	", i, *(p++));
  }
  printf("\n");
  printf("ip host order(small-endian): 0x%x\n", host_num);
  printf("\n");

  printf("ip string: %s\n", inet_ntoa(ip_struct));

  return 0;
}
