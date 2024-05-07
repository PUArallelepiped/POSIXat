#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdatomic.h>

# define MAX_CHAIRS 3
# define MAX_STUDENTS 5

sem_t students;
sem_t ta;
sem_t mutex;
sem_t queue_mutex;

atomic_int waiting_students = 0;
int ta_sleeping = 1;
int help_students = 0;
int student_id = 0;
int chairs[MAX_CHAIRS];
atomic_int turn = -1;

void enqueue(int id) {
    sem_wait(&queue_mutex);
    for (int i = 0; i < MAX_CHAIRS; i++) {
        if (chairs[i] == -1) {
            chairs[i] = id;
            break;
        }
    }
    printf("🪑: %d, 🛋️: %d, 🛏️: %d\n", chairs[0], chairs[1], chairs[2]);
    sem_post(&queue_mutex);
}

int dequeue() {
    sem_wait(&queue_mutex);
    int id = chairs[0];
    for (int i = 0; i < MAX_CHAIRS - 1; i++) {
        chairs[i] = chairs[i + 1];
    }
    chairs[MAX_CHAIRS - 1] = -1;
    printf("🪑: %d, 🛋️: %d, 🛏️: %d\n", chairs[0], chairs[1], chairs[2]);
    sem_post(&queue_mutex);
    return id;
}

void* student(void* param) {
    int id = *((int*)param);
    sem_wait(&mutex);
    while (1) {
        printf("[ST] Student %d comes\n", id);
        // if TA is sleeping, wake him up
        if (ta_sleeping == 1) {
            printf("[ST] Student %d is waking up TA\n", id);
            ta_sleeping = 0;
            turn = id;
            sem_post(&students);
            sem_post(&mutex);
            break;
        } 
        // if there are empty chairs, sit and wait
        else if (waiting_students < MAX_CHAIRS) {
            atomic_fetch_add(&waiting_students, 1);
            enqueue(id);
            printf("[ST] Student %d is waiting\n", id);
            // wait for TA to call, if next turn is not mine, keep waiting
            while (atomic_load(&turn) != id) {
                sem_post(&mutex);
                sleep(1);
                sem_wait(&mutex);
            }
            sem_wait(&ta);
            sem_post(&mutex);
            sem_post(&students);
            atomic_fetch_add(&waiting_students, -1);
            break;
        } 
        // if there are no empty chairs, leave and come back later
        else {
            printf("[ST] Student %d is leaving and coming back later\n", id);
            sem_post(&mutex);
            sleep(rand() % 5 + 3);
        }
    }
}


void* teaching_assistant() {
    while (help_students < MAX_STUDENTS) {
        sem_wait(&mutex);
        // if there are no students, sleep
        if (atomic_load(&waiting_students) <= 0) {
            ta_sleeping = 1;
            printf("[TA] TA is sleeping\n");
            sem_post(&mutex);
            sem_wait(&students);
            // ta_sleeping = 0;
            printf("[TA] TA is waking up\n");
        }
        // if there are students, call the next student
        else {
            printf("[TA] TA is available\n");
            turn = dequeue();
            printf("[TA] next student: %d\n", turn);
            sem_post(&ta);
            printf("[TA] TA is waiting for student\n");
            sem_post(&mutex);
            sem_wait(&students);
        }

        // help student
        printf("[TA] TA is helping student %d\n", turn);
        sleep(rand() % 5 + 1);
        printf("[TA] TA is done helping student %d\n", turn);
        help_students++;
        printf("[TA] helped students: %d\n", help_students);
    }
}


int main(int argc, char *argv[]) {
    sem_init(&students, 0, 0);
    sem_init(&ta, 0, 0);
    sem_init(&mutex, 0, 1);
    sem_init(&queue_mutex, 0, 1);

    pthread_t students[MAX_STUDENTS];
    pthread_t ta;

    int student_ids[MAX_STUDENTS];

    for (int i = 0; i < MAX_CHAIRS; i++) {
        chairs[i] = -1;
    }

    pthread_create(&ta, NULL, teaching_assistant, NULL);

    for (int i = 0; i < MAX_STUDENTS; i++) {
        student_ids[i] = i;
        pthread_create(&students[i], NULL, student, &student_ids[i]);
    }

    for (int i = 0; i < MAX_STUDENTS; i++) {
        pthread_join(students[i], NULL);
    }

    pthread_join(ta, NULL);

    return 0;

}