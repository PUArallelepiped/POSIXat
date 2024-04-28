#include "common.h"
#include <limits.h>
#include "queue.h"
#include "linklist.h"

void insert_data_in_queue(Node **head, int priority, int index)
{
    Node *current = *head;
    short flag = 0;
    while (current != NULL) // find priority group
    {

        if (priority == current->priority)
        {
            enqueue(&current->queue, index);
            flag = 1;
            break;
        }

        current = current->next;
    }

    if (!flag)
    {
        insertSorted(head, priority); // create a new queue
        current = *head;

        while (current != NULL)
        {
            if (priority == current->priority)
            {
                enqueue(&current->queue, index);
                break;
            }

            current = current->next;
        }
    }
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
    Node *head = NULL;

    read_tasks_from_file(argv[1], tasks, &num_tasks);

    int time = 0;
    int quantum = 10;
    int remain_task = num_tasks;
    int task_index = 0;
    int count = 0;

    for (int i = 0; i < num_tasks; i++)
    {

        insert_data_in_queue(&head, tasks[i].priority, i);
    }

    // printList(&head);

    Node *current = head;
    while (current != NULL)
    {
        if ((count % quantum == 0) || (tasks[task_index].remain_burst <= 0)) // every round or finish
        {
            if ((time != 0) && (tasks[task_index].remain_burst > 0)) // task not finish
            {
                enqueue(&current->queue, task_index);
            }

            if (isEmpty(&current->queue) && (tasks[task_index].remain_burst <= 0)) // this priority group finish
            {
                current = current->next;
                if (current == NULL)
                {
                    break;
                }
            }

            task_index = dequeue(&current->queue);
            count = 0;
        }

        printf("time:%3.1d task:%s brust:%2.d priority:%2.d \n", time, tasks[task_index].name, tasks[task_index].remain_burst, tasks[task_index].priority);

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
        count++;
        time++;
    }

    freeList(&head);
    task_detail(tasks, num_tasks, time);
    return 0;
}