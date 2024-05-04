#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

#include "buffer.h"

sem_t empty, full, mutex;

void *producer(void *param) {
    buffer_item item;
    while (1) {
        /* sleep for a random period of time */
        sleep(rand() % 5);
        item = rand() + 1;
        sem_wait(&empty);
        sem_wait(&mutex);
        if (insert_item(item))
            fprintf(stderr, "cannot insert item\n");
        else
            printf("producer produced %d\n",item);
        sem_post(&mutex);
        sem_post(&full);
    }
}

void *consumer(void *param) {
    buffer_item item;
    while (1) {
        sem_wait(&full);
        sem_wait(&mutex);
        /* sleep for a random period of time */
        sleep(rand() % 5);
        if (remove_item(&item))
            fprintf(stderr, "cannot remove item\n");
        else
            printf("consumer consumed %d\n",item);
        sem_post(&mutex);
        sem_post(&empty);
    }
}

int main (int argc, char *argv[]) {
    int sleepTime, producerThreads, consumerThreads;
    pthread_t *producerThread, *consumerThread;
    srand(time(NULL));
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <sleep time> <producer threads> <consumer threads>\n", argv[0]);
        return -1;
    }
    sleepTime = atoi(argv[1]);
    producerThreads = atoi(argv[2]);
    consumerThreads = atoi(argv[3]);
    producerThread = (pthread_t *) malloc(producerThreads * sizeof(pthread_t));
    consumerThread = (pthread_t *) malloc(consumerThreads * sizeof(pthread_t));
    init_buffer();

    sem_init(&empty, 0, BUFFER_SIZE - 1);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    for (int i = 0; i < producerThreads; i++) {
        pthread_create(&producerThread[i], NULL, producer, NULL);
    }
    for (int i = 0; i < consumerThreads; i++) {
        pthread_create(&consumerThread[i], NULL, consumer, NULL);
    }
    sleep(sleepTime);

    return 0;
}