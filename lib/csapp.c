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

/********************************
 * Wrappers for Unix I/O routines
 ********************************/
int Open(const char *pathname, int flags, mode_t mode)
{
  int rc;

  if ((rc = open(pathname, flags, mode)) < 0)
  {
    unix_error("Open error");
  }

  return rc;
}

ssize_t Read(int fd, void *buf, size_t count)
{
  ssize_t rc;

  if ((rc = read(fd, buf, count)) < 0)
  {
    unix_error("Read error");
  }

  return rc;
}

ssize_t Write(int fd, const void *buf, size_t count)
{
  ssize_t rc;

  if ((rc = write(fd, buf, count)) < 0)
  {
    unix_error("Write error");
  }

  return rc;
}

/*********************************************************************
 * The Rio package - robust I/O functions
 **********************************************************************/
/*
 * rio_readn - robustly read n bytes (unbuffered)
 */
 ssize_t rio_readn(int fd, void *usrbuf, size_t n)
 {
   size_t nleft = n;
   ssize_t nread;
   char *bufp = usrbuf;

   while (nleft > 0)
   {
     if ((nread = read(fd, bufp, nleft)) < 0)
     {
       //TODO
       if (errno == EINTR) /* Interrupted by sig handler return */
       {
         nread = 0; /* and call read() again */
       }
       else
       {
         return -1; /* errno set by read() */
       }
     }
     else if (nread == 0)
     {
       break; /* EOF */
     }

     nleft -= nread;
     bufp += nread;
   }

  return (n - nleft); /* Return >= 0 */
 }

/*
 * rio_writen - robustly write n bytes (unbuffered)
 */
 ssize_t rio_writen(int fd, void *usrbuf, size_t n)
 {
   size_t nleft = n;
   ssize_t nwritten;
   char *bufp = usrbuf;

   while (nleft > 0)
   {
     if ((nwritten = write(fd, bufp, nleft)) < 0)
     {
       if (errno == EINTR) /* Interrupted by sig handler return */
       {
         nwritten = 0; /* and call write() again */
       }
       else
       {
         return -1; /* errno set by write() */
       }
     }

     nleft -= nwritten;
     bufp += nwritten;
   }

   return n;
 }

/*
 * rio_read - This is a wrapper for the Unix read() function that
 *    transfers min(n, rio_cnt) bytes from an internal buffer to a user
 *    buffer, where n is the number of bytes requested by the user and
 *    rio_cnt is the number of unread bytes in the internal buffer. On
 *    entry, rio_read() refills the internal buffer via a call to
 *    read() if the internal buffer is empty.
 */
static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n)
{
  int cnt;

  while (rp->rio_cnt <= 0) /* Refill if buf is empty */
  {
    rp->rio_cnt = read(rp->rio_fd, rp->rio_buf,
                       sizeof(rp->rio_buf));
    if (rp->rio_cnt < 0)
    {
      if (errno != EINTR) /* Interrupted by sig handler return */
      {
        return -1;
      }
    }
    else if (rp->rio_cnt == 0) /* EOF */
    {
      return 0;
    }
    else
    {
      rp->rio_bufptr = rp->rio_buf; /* Reset buffer ptr */
    }
  }

  /* Copy min(n, rp->rio_cnt) bytes from internal buf to user buf */
  cnt = n;
  if (rp->rio_cnt < n)
  {
    cnt = rp->rio_cnt;
  }
  memcpy(usrbuf, rp->rio_bufptr, cnt);
  rp->rio_bufptr += cnt;
  rp->rio_cnt -= cnt;

  return cnt;
}

/*
 * rio_readinitb - Associate a descriptor with a read buffer and reset buffer
 */
void rio_readinitb(rio_t *rp, int fd)
{
  rp->rio_fd = fd;
  rp->rio_cnt = 0;
  rp->rio_bufptr = rp->rio_buf;
}

/*
 * rio_readlineb - robustly read a text line (buffered)
 */
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen)
{
  int n, rc;
  char c, *bufp = usrbuf;

  for (n = 1; n < maxlen; n++)
  {
    if ((rc = rio_read(rp, &c, 1)) == 1)
    {
      *bufp++ = c;
      if (c == '\n')
      {
        break;
      }
    }
    else if (rc == 0)
    {
      if (n == 1)
      {
        return 0; /* EOF, no data read */
      }
      else
      {
        break; /* EOF, some data was read */
      }
    }
    else
    {
      return -1; /* Error */
    }
  }
  *bufp = '\0';

  return n;
}

/*
 * rio_readlineb - robustly read a text line (buffered)
 */
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n)
{
  size_t nleft = n;
  ssize_t nread;
  char *bufp = usrbuf;

  while (nleft > 0)
  {
    if ((nread = rio_read(rp, bufp, nleft)) < 0)
    {
      if (errno == EINTR) /* Interrupted by sig handler return */
      {
        nread = 0; /* Call read() again */
      }
      else
      {
        return -1; /* errno set by read() */
      }
    }
    else if (nread == 0)
    {
      break; /* EOF */
    }

    nleft -= nread;
    bufp += nread;
  }

  return (n - nleft); /* Return >= 0*/
}

