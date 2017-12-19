#include "csapp.h"

void *thread(void *vargp);

volatile int cnt = 0; /* Counter */
#ifdef MUTEX_SUPPORT
sem_t mutex; /* Semaphore that protects conuter */
#endif

int main(int argc, char *argv[])
{
  int niters;
  pthread_t tid1, tid2;

  if(argc != 2)
  {
    printf("usage: %s <niters>\n", argv[0]);
  }
  niters = atoi(argv[1]);

#ifdef MUTEX_SUPPORT
  Sem_init(&mutex, 0, 1); /* mutex = 1 */
#endif

  Pthread_create(&tid1, NULL, thread, &niters);
  Pthread_create(&tid2, NULL, thread, &niters);
  Pthread_join(tid1, NULL);
  Pthread_join(tid2, NULL);

  if(cnt != 2 * niters)
  {
    printf("BOOM! cnt=%d\n", cnt);
  }
  else
  {
    printf("OK cnt=%d\n", cnt);
  }
}

void *thread(void *vargp)
{
  int i, niters = *((int *)vargp);

  for(i = 0; i < niters; i++)
  {
#ifdef MUTEX_SUPPORT
    P(&mutex);
    cnt++;
    V(&mutex);
#else
    cnt++;
#endif
  }

  return NULL;
}
