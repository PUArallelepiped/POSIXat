
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "ThreadPool.h"

#define TESTCOUNT 30

typedef struct
{
    int id;
    float size;
} ThreadData;

pthread_mutex_t lock;
int total = 0;

void *randomTask(void *arg)
{
    ThreadData *data = (ThreadData *)arg;

    int waitTime = 1000 * 1000 * data->size;

    usleep(waitTime);
    printf("task %2d:run %4d ms\n", data->id, waitTime / 1000);
    pthread_mutex_lock(&lock);
    total += data->size * 1000;
    pthread_mutex_unlock(&lock);
}
int main()
{

    ThreadData data[TESTCOUNT];

    threadpool_t *pool = pool_init(4, 100);
    for (int i = TESTCOUNT; i >= 0; i--)
    {
        data[i].id = i;
        data[i].size = (float)i / 20.0;

        int fail = pool_submit(pool, (void *)randomTask, &data[i]);
        if (fail == 1)
        {
            printf("Failed to submit task %d\n", i);
        }
    }

    int waitTime = 1000 * 1000 * 1;
    // wait one second
    usleep(waitTime);

    // shutdown the pool
    pool_shutdown(pool);
    printf("run total %dms task in %dms\n", total, waitTime / 1000);

    return 0;
}