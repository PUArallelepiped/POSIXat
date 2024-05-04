
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 256
#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 4

sem_t available_mutex;
int available[NUMBER_OF_RESOURCES];
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int request_resources(int customer_num, int request[]);
void release_resources(int customer_num, int release[]);
int check_safety();
void read_max_from_file(char *filename);

typedef struct {
  int task_num;
} parameters;
void *customer(void *param) {
  parameters *params = (parameters *)param;
  int task_num = params->task_num;
  int req_times = 0;
  int req_times_max = 4;
  while (1) {
    int request[NUMBER_OF_RESOURCES];
    if (req_times < req_times_max) {
      for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        request[i] = rand() % (need[task_num][i] + 1);
      }
    } else {
      for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        request[i] = need[task_num][i];
      }
    }
    sem_wait(&available_mutex);
    printf("Available: ");
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
      printf("%d ", available[i]);
    }
    printf(";");
    printf("Allocation: ");
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
      printf("%d ", allocation[task_num][i]);
    }
    printf(";");
    printf("Need: ");
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
      printf("%d ", need[task_num][i]);
    }
    printf(";");
    printf("Customer %d requesting: ", task_num);
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
      printf("%d ", request[i]);
    }
    printf("\n");
    int result = request_resources(task_num, request);
    if (result == 0) {
      printf("Customer %d request granted\n", task_num);
      req_times++;
    } else {
      printf("Customer %d request denied\n", task_num);
      req_times++;
    }
    sem_post(&available_mutex);

    // sleep(2);
    int compelete = need[task_num][0] == 0 && need[task_num][1] == 0 &&
                    need[task_num][2] == 0 && need[task_num][3] == 0;

    if (allocation[task_num][0] == 0 && allocation[task_num][1] == 0 &&
        allocation[task_num][2] == 0 && allocation[task_num][3] == 0) {
      continue;
    }
    sem_wait(&available_mutex);
    int release[NUMBER_OF_RESOURCES];
    if (req_times >= req_times_max || compelete) {
      for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        release[i] = allocation[task_num][i];
      }
    } else {
      for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        release[i] = rand() % (allocation[task_num][i] + 1);
      }
    }
    printf("Customer %d releasing: ", task_num);
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
      printf("%d ", release[i]);
    }
    printf("\n");
    release_resources(task_num, release);

    sem_post(&available_mutex);
    if (compelete) {
      printf("Customer %d finished\n", task_num);
      break;
    }
  }
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc != 1 + NUMBER_OF_RESOURCES) {
    printf("Usage: %s <resource 1> <resource 2> <resource 3> <resource 4>\n",
           argv[0]);
    return 1;
  }
  for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
    available[i] = atoi(argv[i + 1]);
  }
  read_max_from_file("./customers_max.txt");
  sem_init(&available_mutex, 0, 1);
  pthread_t customers_tid[NUMBER_OF_CUSTOMERS];
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  if (check_safety() != 0) {
    printf("Unsafe state\n");
    return 0;
  }
  for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
    parameters *params = (parameters *)malloc(sizeof(parameters));
    params->task_num = i;
    pthread_create(&customers_tid[i], NULL, customer, (void *)params);
  }
  for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
    pthread_join(customers_tid[i], NULL);
  }

  /*
    char command[10];
    int customer_num;
    int request[NUMBER_OF_RESOURCES];
    while (1) {
      scanf("%s", command);
      if (strcmp(command, "RQ") == 0) {
        scanf("%d", &customer_num);
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
          scanf("%d", &request[i]);
        }
        if (request_resources(customer_num, request) == 0) {
          printf("Request granted\n");
        } else {
          printf("Request denied\n");
        }
      } else if (strcmp(command, "RL") == 0) {
        scanf("%d", &customer_num);
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
          scanf("%d", &request[i]);
        }
        release_resources(customer_num, request);
      } else if (strcmp(command, "*") == 0) {
        // print all
        printf("Available:\n");
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
          printf("%d ", available[i]);
        }
        printf("\n");
        printf("Allocation:\n");
        for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
          for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            printf("%d ", allocation[i][j]);
          }
          printf("\n");
        }
        printf("Need:\n");
        for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
          for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            printf("%d ", need[i][j]);
          }
          printf("\n");
        }
        break;
      }
    }
  */

  return 0;
}
int request_resources(int customer_num, int request[]) {
  for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
    if (request[i] > available[i]) {
      return -1;
    }
  }

  for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
    available[i] -= request[i];
    allocation[customer_num][i] += request[i];
    need[customer_num][i] =
        maximum[customer_num][i] - allocation[customer_num][i];
  }
  if (check_safety() == -1) {
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
      available[i] += request[i];
      allocation[customer_num][i] -= request[i];
      need[customer_num][i] =
          maximum[customer_num][i] - allocation[customer_num][i];
    }
    return -1;
  }
  return 0;
}
void release_resources(int customer_num, int release[]) {
  for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
    available[i] += release[i];
    allocation[customer_num][i] -= release[i];
    need[customer_num][i] =
        maximum[customer_num][i] - allocation[customer_num][i];
  }
}
int check_safety() {
  int work[NUMBER_OF_RESOURCES];
  int finish[NUMBER_OF_CUSTOMERS];
  for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
    work[i] = available[i];
  }
  for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
    finish[i] = 0;
  }
  int count = 0;
  while (count < NUMBER_OF_CUSTOMERS) {
    int flag = 0;
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
      //   printf("check Customer %d\n", i);
      //   for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
      //     printf("%d ", work[j]);
      //   }
      //   printf("\n");
      //   for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
      //     printf("%d ", need[i][j]);
      //   }
      //   printf("\n");
      //   printf("\n");
      if (finish[i] == 0) {
        int j;
        for (j = 0; j < NUMBER_OF_RESOURCES; j++) {
          if (need[i][j] > work[j]) {
            break;
          }
        }
        if (j == NUMBER_OF_RESOURCES) {
          //   printf("check Customer %d finished\n", i);
          for (j = 0; j < NUMBER_OF_RESOURCES; j++) {
            work[j] += allocation[i][j];
          }
          //   printf("check work +++ %d\n", i);
          //   for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
          //     printf("%d ", work[j]);
          //   }
          //   printf("\n");
          finish[i] = 1;
          flag = 1;
          count++;
          break;
        }
      }
    }
    if (flag == 0) {
      return -1;
    }
  }
  return 0;
}
void read_max_from_file(char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("File not found\n");
    exit(1);
  }
  char line[MAX_LINE_LENGTH];
  int customer_num = 0;
  while (fgets(line, MAX_LINE_LENGTH, file)) {
    int resource_num = 0;
    int value;
    char *token = strtok(line, ",");
    while (token != NULL) {
      sscanf(token, "%d", &value);
      maximum[customer_num][resource_num] = value;
      need[customer_num][resource_num] = value;
      token = strtok(NULL, ",");
      resource_num++;
    }
    customer_num++;
  }
  fclose(file);
}