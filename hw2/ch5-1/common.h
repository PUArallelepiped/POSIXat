#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TASKS 1000
#define MAX_LINE_LENGTH 256
typedef struct
{
    char name[64];
    int priority;
    int burst;
    int arrive;
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
        sscanf(line, "%64[^,], %d, %d", tasks[*num_tasks].name, &tasks[*num_tasks].priority, &tasks[*num_tasks].burst);
        (*num_tasks)++;
    }

    fclose(file);
    // return tasks;
}