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
