#include "csapp.h"

int main(int argc, char *argv[])
{
  char **pp;
  #if 0
  struct in_addr {
    unsigned int s_addr; /* Network byte order(big-endian)*/
  };
  #endif
  struct in_addr addr;
  #if 0
  /* Description of data base entry for a single host.  */
  struct hostent
  {
    char *h_name;			/* Official name of host.  */
    char **h_aliases;		/* Alias list.  */
    int h_addrtype;		/* Host address type.  */
    int h_length;			/* Length of address.  */
    char **h_addr_list;		/* List of addresses from name server.  */
  };
  #endif

  struct hostent *hostp;

  if (argc != 2)
  {
    fprintf(stderr, "usage: %s <domain name or dotted-decimal>\n", argv[0]);

    exit(0);
  }

  if (inet_aton(argv[1], &addr) != 0)
  {
    hostp = Gethostbyaddr((const char *)&addr, sizeof(addr), AF_INET);
  }
  else
  {
    hostp = Gethostbyname(argv[1]);
  }

  printf("official hostname: %s\n", hostp->h_name);

  for (pp = hostp->h_aliases; *pp != NULL; pp++)
  {
    printf("alias: %s\n", *pp);
  }

  for (pp = hostp->h_addr_list; *pp != NULL; pp++)
  {
    addr.s_addr = ((struct in_addr *)(*pp))->s_addr;
    printf("address: %s\n", inet_ntoa(addr));
  }

  exit(0);
}
