
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 10

int list[SIZE] = {0};
int list2[SIZE] = {0};
typedef struct {
  int begin;
  int end;
} parameters;

void *sort(void *param) {
  parameters *params = (parameters *)param;
  printf("before sorted: ");
  for (int i = params->begin; i < params->end; i++) {
    printf("%d ", list[i]);
  }
  // gnome sort
  int index = params->begin + 1;
  while (index < params->end) {
    if (list[index] >= list[index - 1]) {
      index++;
    } else {
      int temp = list[index];
      list[index] = list[index - 1];
      list[index - 1] = temp;
      index--;
      if (index == params->begin) {
        index++;
      }
    }
  }
  printf("\nafter  sorted: ");
  for (int i = params->begin; i < params->end; i++) {
    printf("%d ", list[i]);
  }
  printf("\n");
  printf("\n");

  return 0;
};
void *merge(void *param) {

  printf("merge\n");
  int mergedIndex = 0;
  int frontIndex = 0;
  int backIndex = SIZE / 2;
  while (1) {
    // printf("frontIndex: %d, backIndex: %d\n", frontIndex, backIndex);
    if (frontIndex == SIZE / 2 && backIndex == SIZE) {
      break;
    }
    if (frontIndex >= SIZE / 2) {
      list2[mergedIndex] = list[backIndex];
      backIndex++;
    } else if (backIndex >= SIZE) {
      list2[mergedIndex] = list[frontIndex];
      frontIndex++;
    } else if (list[frontIndex] < list[backIndex]) {
      list2[mergedIndex] = list[frontIndex];
      frontIndex++;
    } else {
      list2[mergedIndex] = list[backIndex];
      backIndex++;
    }
    mergedIndex++;
  }
  return 0;
};

int main() {
  pthread_t tid1, tid2, tid3;
  pthread_attr_t attr;
  pthread_attr_init(&attr);

  srand(time(NULL));
  for (int i = 0; i < SIZE; i++) {
    list[i] = rand() % 100;
  }

  for (int i = 0; i < SIZE; i++) {
    printf("%d ", list[i]);
  }
  printf("\n");

  parameters p1 = {0, SIZE / 2};
  parameters p2 = {SIZE / 2, SIZE};

  pthread_create(&tid1, &attr, sort, &p1);
  pthread_create(&tid2, &attr, sort, &p2);

  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);

  pthread_create(&tid3, &attr, merge, NULL);
  pthread_join(tid3, NULL);

  for (int i = 0; i < SIZE; i++) {
    printf("%d ", list2[i]);
  }
  printf("\n");

  return 0;
}