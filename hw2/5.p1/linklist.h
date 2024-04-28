#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int priority;
    Queue queue; // for rr data
    struct Node *next;
} Node;

void insertSorted(Node **_head, int _priority)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    newNode->priority = _priority;
    initQueue(&newNode->queue);
    newNode->next = NULL;

    Node **head = _head;

    // sort with priority

    if (*head == NULL || _priority > (*head)->priority)
    {
        newNode->next = *head;
        *head = newNode;
        return;
    }

    Node *current = *_head;
    while (current->next != NULL && current->next->priority > _priority)
    {
        current = current->next;
    }
    newNode->next = current->next;
    current->next = newNode;
}

// print queue in list

void printList(Node **head)
{
    Node *current = *head;
    while (current != NULL)
    {
        printf("%d ", current->priority);
        printQueue(&current->queue);
        current = current->next;
    }
    printf("\n");
}

void freeList(Node **head)
{
    Node *current = *head;
    while (current != NULL)
    {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
}
