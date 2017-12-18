#include "csapp.h"

/************************** 
 * Error-handling functions
 **************************/
void posix_error(int code, char *msg) /* posix-style error */
{
    fprintf(stderr, "%s: %s\n", msg, strerror(code));
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

