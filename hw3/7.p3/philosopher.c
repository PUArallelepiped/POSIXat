#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5
#define LEFT (i + N - 1) % N
#define RIGHT (i + 1) % N
#define THINKING 0
#define HUNGRY 1
#define EATING 2

int state[N];
pthread_cond_t cond[N];
pthread_mutex_t mutex;

void print_dining_table() {
    int i;
    for (i = 0; i < N; i++) {
        if (state[i] == THINKING) {
            printf("T ");
        } else if (state[i] == HUNGRY) {
            printf("H ");
        } else {
            printf("E ");
        }
    }
    printf("\n");
}

void test(int i) {
    if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
        state[i] = EATING;
        print_dining_table();
        pthread_cond_signal(&cond[i]);
    }
}

void take_forks(int i) {
    pthread_mutex_lock(&mutex);
    state[i] = HUNGRY;
    print_dining_table();
    test(i);
    while (state[i] != EATING) {
        pthread_cond_wait(&cond[i], &mutex);
    }
    pthread_mutex_unlock(&mutex);
}

void put_forks(int i) {
    pthread_mutex_lock(&mutex);
    state[i] = THINKING;
    print_dining_table();
    test(LEFT);
    test(RIGHT);
    pthread_mutex_unlock(&mutex);
}

void *philosopher(void* arg) {
    int i = *(int *)arg;
    while (1) {
        printf("Philosopher %d is thinking\n", i);
        sleep(rand() % 3 + 1);
        take_forks(i);
        printf("Philosopher %d is eating\n", i);
        sleep(rand() % 3 + 1);
        put_forks(i);
    }
}

int main() {
    pthread_t tid[N];
    int i;
    for (i = 0; i < N; i++) {
        state[i] = THINKING;
    }
    for (i = 0; i < N; i++) {
        pthread_cond_init(&cond[i], NULL);
    }
    pthread_mutex_init(&mutex, NULL);
    for (i = 0; i < N; i++) {
        int *arg = (int *)malloc(sizeof(*arg));
        *arg = i;
        pthread_create(&tid[i], NULL, philosopher, arg);
    }
    for (i = 0; i < N; i++) {
        pthread_join(tid[i], NULL);
    }
    return 0;
}