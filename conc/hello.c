#include "csapp.h" 

void *thread1(void *vargp); /* Thread routine 1 */
void *thread2(void *vargp); /* Thread routine 2 */

int main()
{
  pthread_t tid1, tid2;
  Pthread_create(&tid1, NULL, thread1, NULL);
  Pthread_create(&tid2, NULL, thread2, NULL);
  printf("This is main thread, "
          "my tid: %d, child 1 tid: %d, child 2 tid: %d\n",
          Pthread_self(), tid1, tid2);
  Pthread_join(tid2, NULL);

  printf("Main thread exit\n");
  exit(0);
}

void *thread1(void *vargp) /* Thread routine 1 */
{
  Pthread_detach(Pthread_self());

  printf("This is child thread 1, my tid: %d\n", Pthread_self());

  printf("Hello, world 1!\n");

  return NULL;
}

void *thread2(void *vargp) /* Thread routine 2 */
{
  printf("This is child thread 2, my tid: %d\n", Pthread_self());

  printf("Hello, world 2!\n");

  return NULL;
}
