#include "common.h"

int next_task(Task *tasks, int num_tasks, int time)
{
    int task_index = -1;
    for (int i = 0; i < num_tasks; i++)
    {
        if (tasks[i].burst > 0)
        {
            task_index = i;
            break;
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
    //     printf("%s %d %d\n", tasks[i].name, tasks[i].priority, tasks[i].burst);
    // }

    int time = 1;
    int remain_task = num_tasks;
    while (remain_task > 0)
    {

        int task_index = next_task(tasks, num_tasks, time);

        if (task_index != -1)
        {
            printf("time:%3.d task:%s\n", time, tasks[task_index].name);
            tasks[task_index].burst--;
            if (tasks[task_index].burst == 0)
                remain_task--;
        }

        time++;
    }
    return 0;
}