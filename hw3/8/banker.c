// banker's algorithm

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256
#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 4

int available[NUMBER_OF_RESOURCES];
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int request_resources(int customer_num, int request[]);
void release_resources(int customer_num, int release[]);
int check_safety(int ava[NUMBER_OF_RESOURCES]);
void read_max_from_file(char *filename);

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
  if (check_safety(available) == -1) {
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
int check_safety(int ava[NUMBER_OF_RESOURCES]) {
  int work[NUMBER_OF_RESOURCES];
  int finish[NUMBER_OF_CUSTOMERS];
  for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
    work[i] = ava[i];
  }
  for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
    finish[i] = 0;
  }
  int count = 0;
  while (count < NUMBER_OF_CUSTOMERS) {
    int flag = 0;
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
      if (finish[i] == 0) {
        int j;
        for (j = 0; j < NUMBER_OF_RESOURCES; j++) {
          if (need[i][j] > work[j]) {
            break;
          }
        }
        if (j == NUMBER_OF_RESOURCES) {
          for (j = 0; j < NUMBER_OF_RESOURCES; j++) {
            work[j] += allocation[i][j];
          }
          finish[i] = 1;
          flag = 1;
          count++;
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