
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct
{
  void (*function)(void *);
  void *argument;
} threadpool_task_t;

typedef struct
{
  pthread_mutex_t lock;
  pthread_cond_t notify;
  pthread_t *threads;
  threadpool_task_t *queue;
  int thread_size;
  int queue_size;

  int task_count;
  int running_threads;
  int kill;

} threadpool_t;

void worker(void *threadpool);
int pool_shutdown(threadpool_t *pool);

threadpool_t *pool_init(int thread_size, int queue_size)
{
  threadpool_t *pool = malloc(sizeof(threadpool_t));

  pthread_mutex_init(&(pool->lock), NULL);
  pthread_cond_init(&(pool->notify), NULL);

  pool->queue = malloc(sizeof(threadpool_task_t) * queue_size);
  pool->threads = malloc(sizeof(pthread_t) * thread_size);

  pool->thread_size = thread_size;
  pool->queue_size = queue_size;
  pool->task_count = 0;
  pool->kill = 0;

  for (int i = 0; i < thread_size; i++)
  {
    if (pthread_create(&(pool->threads[i]), NULL,
                       (void *)worker, (void *)pool) != 0)
    {
      printf("Failed to create thread\n");
      pool_shutdown(pool);
      return NULL;
    }
    else
    {
      pool->running_threads++;
    }
  }

  return pool;
}

int pool_submit(threadpool_t *pool, void (*function)(void *), void *argument)
{

  pthread_mutex_lock(&pool->lock);
  if (pool->task_count >= pool->queue_size || pool->kill == 1)
  {
    return 1;
  }

  pool->queue[pool->task_count].function = function;
  pool->queue[pool->task_count].argument = argument;

  pool->task_count++;

  // wake up one thread that is waiting
  pthread_cond_signal(&(pool->notify));

  pthread_mutex_unlock(&pool->lock);

  return 0;
}
int pool_shutdown(threadpool_t *pool)
{

  pool->kill = 1;
  if (pthread_cond_broadcast(&pool->notify) != 0)
  {
    printf("Failed to broadcast\n");
    return 1;
  }
  if (pool == NULL || pool->running_threads > 0)
  {
    return 1;
  }

  if (pool->threads)
  {
    free(pool->threads);
    free(pool->queue);

    pthread_mutex_lock(&(pool->lock));
    pthread_mutex_destroy(&(pool->lock));
    pthread_cond_destroy(&(pool->notify));
  }

  free(pool);
  return 0;
}

void worker(void *threadpool)
{
  threadpool_t *pool = (threadpool_t *)threadpool;
  threadpool_task_t task;

  while (1)
  {

    pthread_mutex_lock(&pool->lock);

    while (pool->task_count <= 0)
    {
      pthread_cond_wait(&pool->notify, &pool->lock);
    }
    if (pool->kill == 1)
    {
      break;
    }

    // execute task
    task = pool->queue[--pool->task_count];
    pthread_mutex_unlock(&pool->lock);

    task.function(task.argument);
  }
  pool->running_threads--;
}
