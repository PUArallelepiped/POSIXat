#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TASKS 1000
#define MAX_LINE_LENGTH 256
typedef struct
{
    char name[64];
    int priority;
    int arrive;
    int burst;

    int remain_burst;
    int start;
    int end;
} Task;

void read_tasks_from_file(const char *filename, Task *tasks, int *num_tasks)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    if (!tasks)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    *num_tasks = 0;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, file) && *num_tasks < MAX_TASKS)
    {
        tasks[*num_tasks].arrive = 0;
        tasks[*num_tasks].start = -1;
        tasks[*num_tasks].end = -1;
        // get first 64 characters, then an integer, then another integer
        sscanf(line, "%64[^,], %d, %d", tasks[*num_tasks].name, &tasks[*num_tasks].priority, &tasks[*num_tasks].burst);
        tasks[*num_tasks].remain_burst = tasks[*num_tasks].burst;
        (*num_tasks)++;
    }

    fclose(file);
    // return tasks;
}
void task_detail(Task *tasks, int num_tasks, int time)
{
    float total_turnaround_time = 0;
    float total_waiting_time = 0;
    float total_response_time = 0;
    for (int i = 0; i < num_tasks; i++)
    {
        float turnaround_time = tasks[i].end - tasks[i].arrive;
        float waiting_time = turnaround_time - tasks[i].burst;
        float response_time = tasks[i].start - tasks[i].arrive;

        total_turnaround_time += turnaround_time;
        total_waiting_time += waiting_time;
        total_response_time += response_time;
        printf("%s turnaround_time:%2.f waiting_time:%2.f response_time:%2.f\n", tasks[i].name, turnaround_time, waiting_time, response_time);
    }
    printf("Average turnaround time: %.1f\n", total_turnaround_time / num_tasks);
    printf("Average waiting time: %.1f\n", total_waiting_time / num_tasks);
    printf("Average response time: %.1f\n", total_response_time / num_tasks);
}