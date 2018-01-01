/* $begin csapp.h */
#ifndef __CSAPP_H__
#define __CSAPP_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
/* Network */
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

/* Simplifies calls to bind(), connect(), and accept() */
typedef struct sockaddr SA;

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

#endif
