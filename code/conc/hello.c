#include <stdio.h> 
#include <pthread.h>
#include <stdlib.h>

void *thread(void *vargp); /* Thread routine */

int main()
{
  pthread_t tid;
  printf("This is main thread\n");
  pthread_create(&tid, NULL, thread, NULL);
  printf("Main tid: %d, child tid: %d\n", pthread_self(), tid);
  pthread_join(tid, NULL);

  printf("Main thread exit\n");
  exit(0);
}

void *thread(void *vargp) /* Thread routine */
{
  printf("This is child thread, my tid: %d\n", pthread_self());
  pthread_detach(pthread_self());

  if(!vargp)
  {
    printf("no args\n");
  }

  printf("Hello, world!\n");
  return NULL;
}
