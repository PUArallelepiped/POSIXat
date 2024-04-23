#ifndef QUEUE_H
#define QUEUE_H
#define MAX_SIZE 1000

typedef struct
{
    int task_index[MAX_SIZE];
    int front;
    int rear;
} Queue;

void initQueue(Queue *queue)
{
    queue->front = 0;
    queue->rear = -1;
}

int isEmpty(Queue *queue)
{
    return queue->rear < queue->front;
}

int isFull(Queue *queue)
{
    return queue->rear >= MAX_SIZE - 1;
}

void enqueue(Queue *queue, int value)
{
    if (isFull(queue))
    {
        return;
    }
    queue->task_index[++queue->rear] = value;
}

int dequeue(Queue *queue)
{
    if (isEmpty(queue))
    {
        exit(EXIT_FAILURE);
    }
    return queue->task_index[queue->front++];
}

void printQueue(Queue *queue)
{
    printf("Queue elements: ");
    for (int i = queue->front; i <= queue->rear; i++)
    {
        printf("%d ", queue->task_index[i]);
    }
    printf("\n");
}

#endif