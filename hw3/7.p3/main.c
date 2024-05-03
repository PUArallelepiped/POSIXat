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

atomic_int waiting_students = 0;
int ta_sleeping = 1;
int help_students = 0;
int student_id = 0;
int chairs[MAX_CHAIRS];
int next_seat = 0;
int turn = -1;

void enqueue(int id) {
    sem_wait(&mutex);
    chairs[next_seat] = id;
    next_seat = (next_seat + 1) % MAX_CHAIRS;
    sem_post(&mutex);
}

int dequeue() {
    sem_wait(&mutex);
    int id = chairs[0];
    for (int i = 0; i < MAX_CHAIRS - 1; i++) {
        chairs[i] = chairs[i + 1];
    }
    chairs[MAX_CHAIRS - 1] = -1;
    sem_post(&mutex);
    return id;
}

void* student(void* param) {
    int id = *((int*)param);
    printf("Student %d comes\n", id);
    sem_wait(&mutex);
    while (1) {
        if (ta_sleeping == 1) {
            printf("Student %d is waking up TA\n", id);
            ta_sleeping = 0;
            sem_post(&students);
            sem_post(&mutex);
            break;
        } 
        else if (waiting_students < MAX_CHAIRS) {
            atomic_fetch_add(&waiting_students, 1);
            chairs[next_seat] = id;
            next_seat = (next_seat + 1) % MAX_CHAIRS;
            printf("Student %d is waiting\n", id);
            printf("chair 1: %d, chair 2: %d, chair 3: %d\n", chairs[0], chairs[1], chairs[2]);
            sem_post(&mutex);
            sem_wait(&ta);
            sem_post(&students);
            atomic_fetch_add(&waiting_students, -1);
            break;
        } 
        else {
            printf("Student %d is leaving and coming back later\n", id);
            sem_post(&mutex);
            sleep(rand() % 5 + 1);
        }
    }
    printf("Student %d is getting help\n", id);
}


void* teaching_assistant() {
    while (help_students < MAX_STUDENTS) {
        sem_wait(&mutex);
        if (atomic_load(&waiting_students) == 0) {
            ta_sleeping = 1;
            printf("TA is sleeping\n");
            sem_post(&mutex);
            sem_wait(&students);
            // ta_sleeping = 0;
            printf("TA is waking up\n");
        }
        else {
            printf("TA is available\n");
            sem_post(&ta);
            printf("TA is waiting for student\n");
            sem_wait(&students);
            sem_post(&mutex);
        }

        printf("TA is helping student\n");
        help_students++;
        printf("helped students: %d\n", help_students);
        sleep(rand() % 5 + 1);
        printf("TA is done helping student\n");
        printf("waiting students: %d\n", waiting_students);
    }
}


int main(int argc, char *argv[]) {
    sem_init(&students, 0, 0);
    sem_init(&ta, 0, 0);
    sem_init(&mutex, 0, 1);

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