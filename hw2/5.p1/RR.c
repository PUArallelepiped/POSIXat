#include "common.h"
#include <limits.h>
#include "queue.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <input file>\n", argv[0]);
        exit(1);
    }
    int num_tasks = 0;
    Task tasks[1000];
    Queue queue;

    read_tasks_from_file(argv[1], tasks, &num_tasks);
    initQueue(&queue);

    int time = 0;
    int quantum = 10;
    int remain_task = num_tasks;
    int task_index = 0;
    int count = 0;

    for (int i = 0; i < num_tasks; i++)
    {
        enqueue(&queue, i);
    }

    while (remain_task > 0)
    {
        if ((count % quantum == 0) || (tasks[task_index].remain_burst <= 0))
        {
            if ((time != 0) && (tasks[task_index].remain_burst > 0))
            {
                enqueue(&queue, task_index);
            }
            task_index = dequeue(&queue);
            count = 0;
        }

        printf("time:%3.1d task:%s brust:%2.d\n", time, tasks[task_index].name, tasks[task_index].remain_burst);

        tasks[task_index].remain_burst--;

        if (tasks[task_index].start == -1)
        {
            tasks[task_index].start = time;
        }

        if (tasks[task_index].remain_burst <= 0)
        {
            tasks[task_index].end = time + 1;
            remain_task--;
        }

        time++;
        count++;
    }
    task_detail(tasks, num_tasks, time);
    return 0;
}