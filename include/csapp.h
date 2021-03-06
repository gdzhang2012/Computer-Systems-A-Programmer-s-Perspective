/* $begin csapp.h */
#ifndef __CSAPP_H__
#define __CSAPP_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* pthread */
#include <pthread.h>
#include <semaphore.h>

/* Network */
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

/* IO */
#include <sys/stat.h>

/* Default file permissions are DEF_MODE & ~DEF_UMASK */
/* S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH */
#define DEF_MODE   S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH
#define DEF_UMASK  S_IWGRP|S_IWOTH

/* Simplifies calls to bind(), connect(), and accept() */
typedef struct sockaddr SA;

/* Persistent state for the robust I/O (Rio) package */
#define RIO_BUFSIZE 8192
typedef struct {
  int rio_fd;                   /* Descriptor for this internal buf */
  int rio_cnt;                  /* Unread bytes in internal buf */
  char *rio_bufptr;             /* Next unread byte in internal buf */
  char rio_buf[RIO_BUFSIZE];    /* Internal buffer */
} rio_t;

/* External variables */
extern int h_errno;    /* defined by BIND for DNS errors */

/* Misc constants */
#define LISTENQ  1024  /* second argument to listen() */

/* Our own error-handling functions */
void unix_error(char *msg);
void posix_error(int code, char *msg);
void dns_error(char *msg);

/* Pthreads thread control wrappers */
void Pthread_create(pthread_t *tidp, pthread_attr_t *attrp,
        void * (*routine)(void *), void *argp);
void Pthread_join(pthread_t tid, void **thread_return);
void Pthread_cancel(pthread_t tid);
void Pthread_detach(pthread_t tid);
void Pthread_exit(void *retval);
pthread_t Pthread_self(void);
void Pthread_once(pthread_once_t *once_control, void (*init_function)());

/* sem functions wrappers */
void P(sem_t *s);
void S(sem_t *s);

/* DNS wrappers */
struct hostent *Gethostbyaddr(const char *addr, int len, int type);
struct hostent *Gethostbyname(const char *name);

/* Sockets interface wrappers */
int Socket(int domain, int type, int protocol);

/* Client/server helper functions */
int open_clientfd(char *hostname, int portno);
int open_listenfd(int portno);

/* Wrappers for client/server helper functions */
int Open_clientfd(char *hostname, int port);
int Open_listenfd(int port);

/* Rio (Robust I/O) package */
ssize_t rio_readn(int fd, void *usrbuf, size_t n);
ssize_t rio_writen(int fd, void *usrbuf, size_t n);
void rio_readinitb(rio_t *rp, int fd);
ssize_t	rio_readnb(rio_t *rp, void *usrbuf, size_t n);
ssize_t	rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);

/* Unix I/O wrappers */
int Open(const char *pathname, int flags, mode_t mode);
ssize_t Read(int fd, void *buf, size_t count);
ssize_t Write(int fd, const void *buf, size_t count);
off_t Lseek(int fildes, off_t offset, int whence);
void Close(int fd);
//int Select(int  n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
//	   struct timeval *timeout);
//int Dup2(int fd1, int fd2);
void Stat(const char *filename, struct stat *buf);
//void Fstat(int fd, struct stat *buf) ;

#endif
