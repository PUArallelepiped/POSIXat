#include "common.h"
#include <limits.h>
int next_task(Task *tasks, int num_tasks, int time)
{
    int task_index = -1;
    int min_remain_burst = INT_MAX;
    for (int i = 0; i < num_tasks; i++)
    {
        if (tasks[i].remain_burst > 0 && tasks[i].remain_burst < min_remain_burst)
        {
            task_index = i;
            min_remain_burst = tasks[i].remain_burst;
        }
    }
    return task_index;
}
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <input file>\n", argv[0]);
        exit(1);
    }
    int num_tasks = 0;
    Task tasks[1000];

    read_tasks_from_file(argv[1], tasks, &num_tasks);

    // for (int i = 0; i < num_tasks; i++)
    // {
    //     printf("%s %d %d %d\n", tasks[i].name, tasks[i].priority, tasks[i].burst, tasks[i].arrive);
    // }

    int time = 0;
    int remain_task = num_tasks;
    while (remain_task > 0)
    {
        int task_index = next_task(tasks, num_tasks, time);

        if (task_index != -1)
        {
            printf("time:%3.1d task:%s remain:%2.d\n", time, tasks[task_index].name, tasks[task_index].remain_burst);
            // printf("time:%3.d task:%s\n", time, tasks[task_index].name);

            tasks[task_index].remain_burst--;
            if (tasks[task_index].start == -1)
                tasks[task_index].start = time;

            if (tasks[task_index].remain_burst <= 0)
            {
                tasks[task_index].end = time + 1;
                remain_task--;
            }
        }
        time++;
    }
    task_detail(tasks, num_tasks, time);
    return 0;
}