#include "csapp.h"

/************************** 
 * Error-handling functions
 **************************/
void posix_error(int code, char *msg) /* posix-style error */
{
  fprintf(stderr, "%s: %s\n", msg, strerror(code));
  exit(0);
}

void unix_error(char *msg) /* unix-style error */
{
  fprintf(stderr, "%s: %s\n", msg, strerror(errno));
  exit(0);
}

void dns_error(char *msg) /* dns-style error */
{
    fprintf(stderr, "%s: DNS error %d\n", msg, h_errno);
    exit(0);
}


/************************************************
 * Wrappers for Pthreads thread control functions
 ************************************************/
void Pthread_create(pthread_t *tidp, pthread_attr_t *attrp, 
             void * (*routine)(void *), void *argp)
{
  int rc;

  if ((rc = pthread_create(tidp, attrp, routine, argp)) != 0)
  {
    posix_error(rc, "Pthread_create error");
  }
}

void Pthread_cancel(pthread_t tid)
{
  int rc;

  if ((rc = pthread_cancel(tid)) != 0)
  {
    posix_error(rc, "Pthread_cancel error");
  }
}

void Pthread_join(pthread_t tid, void **thread_return)
{
  int rc;

  if ((rc = pthread_join(tid, thread_return)) != 0)
  {
    posix_error(rc, "Pthread_join error");
  }
}

void Pthread_detach(pthread_t tid)
{
  int rc;

  if ((rc = pthread_detach(tid)) != 0)
  {
    posix_error(rc, "Pthread_detach error");
  }
}

void Pthread_exit(void *retval)
{
  pthread_exit(retval);
}

pthread_t Pthread_self(void)
{
  return pthread_self();
}
 
void Pthread_once(pthread_once_t *once_control, void (*init_function)())
{
  pthread_once(once_control, init_function);
}

/*******************************
 * Wrappers for Posix semaphores
 *******************************/
void Sem_init(sem_t *sem, int pshared, unsigned int value)
{
  if (sem_init(sem, pshared, value) < 0)
  {
    unix_error("Sem_init error");
  }
}

void P(sem_t *sem)
{
  if (sem_wait(sem) < 0)
  {
    unix_error("P error");
  }
}

void V(sem_t *sem)
{
  if (sem_post(sem) < 0)
  {
    unix_error("V error");
  }
}

/************************
 * DNS interface wrappers 
 ***********************/
struct hostent *Gethostbyaddr(const char *addr, int len, int type)
{
  struct hostent *p;

  if ((p = gethostbyaddr(addr, len, type)) == NULL)
  {
    dns_error("Gethostbyaddr error");
  }

  return p;
}

struct hostent *Gethostbyname(const char *name)
{
  struct hostent *p;

  if ((p = gethostbyname(name)) == NULL)
  {
    dns_error("Gethostbyname error");
  }

  return p;
}

/**************************** 
 * Sockets interface wrappers
 ****************************/
int Socket(int domain, int type, int protocol)
{
  int rc;

  if ((rc = socket(domain, type, protocol)) < 0)
  {
    unix_error("Socket error");
  }

  return rc;
}

/******************************** 
 * Client/server helper functions
 ********************************/
/*
 * open_clientfd - open connection to server at <hostname, port> 
 *   and return a socket descriptor ready for reading and writing.
 *   Returns -1 and sets errno on Unix error. 
 *   Returns -2 and sets h_errno on DNS (gethostbyname) error.
 */
 int open_clientfd(char *hostname, int port)
 {
   int clientfd;
   struct hostent *hp;
   struct sockaddr_in serveraddr;

   if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
   {
     return -1; /* Check errno for cause of error */
   }

   /* Fill in the server's IP address and port */
   if ((hp = gethostbyname(hostname)) == NULL)
   {
     return -2; /* Check h_errno for cause of error */
   }
   bzero((char *)&serveraddr, sizeof(serveraddr));
   serveraddr.sin_family = AF_INET;
   bcopy((char *)hp->h_addr_list[0],
        (char *)&serveraddr.sin_addr.s_addr, hp->h_length);
   serveraddr.sin_port = htons(port);

   /* Establish a connection with the server */
   if (connect(clientfd, (SA *)&serveraddr, sizeof(serveraddr)) < 0)
   {
     return -1;
   }

   return clientfd;
 }

/*  
 * open_listenfd - open and return a listening socket on port
 *     Returns -1 and sets errno on Unix error.
 */
 int open_listenfd(int port)
 {
   int listenfd, optval = 1;
   struct sockaddr_in serveraddr;

   /* Create a socket descriptor */
   if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
   {
     return -1;
   }

   /* Eliminates "Address already in use" error from bind */
   if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
                  (const void *)&optval, sizeof(int)) < 0)
   {
     return -1;
   }

   /* Listenfd will be an end point for all requests to port
      on any IP address for this host */
   bzero((char *)&serveraddr, sizeof(serveraddr));
   serveraddr.sin_family = AF_INET;
   serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
   serveraddr.sin_port = htons((unsigned short)port);
   if (bind(listenfd, (SA *)&serveraddr, sizeof(serveraddr)) < 0)
   {
     return -1;
   }

   /* Make it a listening socket ready to accept connection requests */
   if (listen(listenfd, LISTENQ) < 0)
   {
     return -1;
   }

  return listenfd;
 }

/******************************************
 * Wrappers for the client/server helper routines 
 ******************************************/
int Open_clientfd(char *hostname, int port) 
{
    int rc;

    if ((rc = open_clientfd(hostname, port)) < 0)
    {
      if (rc == -1)
      {
        unix_error("Open_clientfd Unix error");
      }
	    else        
      {
	      dns_error("Open_clientfd DNS error");
      }
    }

    return rc;
}

int Open_listenfd(int port) 
{
    int rc;

    if ((rc = open_listenfd(port)) < 0)
    {
      unix_error("Open_listenfd error");
    }

    return rc;
}
