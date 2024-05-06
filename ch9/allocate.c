#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define INT_MAX 2147483647

struct Node
{
    int available_space; // The available space in this segment
    int start_address;   // The start address
    int end_address;     // The end address
    int process_id;      // The process ID
    struct Node *next;   // Pointer to the next node
};

void init_memory(struct Node **head_ref, int _available_space)
{

    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    new_node->available_space = _available_space;
    new_node->process_id = -1;
    new_node->next = NULL;

    new_node->start_address = 0;
    new_node->end_address = _available_space - 1;
    *head_ref = new_node;
    return;
}

void sep_node(struct Node **preious_ref, struct Node **avaliable_ref, struct Node **new_ref)
{
    struct Node *preious = *preious_ref;
    struct Node *avaliable = *avaliable_ref;
    struct Node *new = *new_ref;

    avaliable->available_space -= new->available_space;
    new->start_address = avaliable->start_address;
    new->end_address = new->start_address + new->available_space - 1;
    avaliable->start_address = new->end_address + 1;

    new->next = avaliable;

    if (*preious_ref == *avaliable_ref)
    {

        *preious_ref = new;
        return;
    }

    preious->next = new;
}

void combine_node(struct Node **first_ref, struct Node **second_ref)
{
    struct Node *first = *first_ref;
    struct Node *second = *second_ref;

    first->available_space += second->available_space;
    first->end_address += second->available_space;
    first->next = second->next;

    free(second);
}

void F_insert(struct Node **head_ref, int _available_space, int _process_id)
{
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));

    new_node->available_space = _available_space;
    new_node->process_id = _process_id;
    new_node->next = NULL;

    struct Node *current = *head_ref;
    struct Node *perious = *head_ref;

    while (current != NULL)
    {

        if ((current->process_id == -1) && (new_node->available_space < current->available_space))
        {
            if (perious == current && perious == *head_ref)
            {
                sep_node(head_ref, &current, &new_node); // if is head
            }
            else
            {
                sep_node(&perious, &current, &new_node);
            };
            return;
        }
        else if ((current->process_id == -1) && (new_node->available_space == current->available_space))
        {
            current->process_id = _process_id;
            free(new_node);

            return;
        }
        perious = current;
        current = current->next;
    }
}

void B_insert(struct Node **head_ref, int _available_space, int _process_id)
{
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));

    new_node->available_space = _available_space;
    new_node->process_id = _process_id;
    new_node->next = NULL;

    struct Node *current = *head_ref;
    struct Node *perious = *head_ref;
    struct Node *best = NULL;
    struct Node *best_pre = NULL;
    int diff = INT_MAX;
    while (current != NULL)
    {

        if ((current->process_id == -1) && (new_node->available_space <= current->available_space))
        {
            int _diff = current->available_space - new_node->available_space;

            if ((_diff < diff))
            {
                best_pre = perious;
                best = current;
                diff = _diff;
            }
        }
        else if ((current->process_id == -1) && (new_node->available_space == current->available_space))
        {
            current->process_id = _process_id;
            free(new_node);
            return;
        }
        perious = current;
        current = current->next;
    }

    if (best != NULL)
    {
        if (diff == 0)
        {
            best->process_id = _process_id;
            free(new_node);
            return;
        }
        if ((best_pre == best) && (best_pre == *head_ref))
        {
            sep_node(head_ref, &best, &new_node); // if is head
            return;
        }
        sep_node(&best_pre, &best, &new_node);
    }
}

void W_insert(struct Node **head_ref, int _available_space, int _process_id)
{
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));

    new_node->available_space = _available_space;
    new_node->process_id = _process_id;
    new_node->next = NULL;

    struct Node *current = *head_ref;
    struct Node *perious = *head_ref;
    struct Node *best = NULL;
    struct Node *best_pre = NULL;
    int MAX = -1;
    while (current != NULL)
    {

        if ((current->process_id == -1) && (new_node->available_space <= current->available_space))
        {
            int _diff = current->available_space - new_node->available_space;
            if ((_diff > MAX))
            {
                best_pre = perious;
                best = current;
                MAX = _diff;
            }
        }

        perious = current;
        current = current->next;
    }

    if (best != NULL)
    {
        if (MAX == 0)
        {
            best->process_id = _process_id;
            free(new_node);
            return;
        }
        if ((best_pre == best) && (best_pre == *head_ref))
        {
            sep_node(head_ref, &best, &new_node); // if is head
            return;
        }

        sep_node(&best_pre, &best, &new_node);
    }
}

void RL_release_node(struct Node **head_ref, int _process_id)
{
    struct Node *current = *head_ref;
    while (current != NULL)
    {
        if ((current->process_id == _process_id))
        {
            current->process_id = -1;
            return;
        }
        current = current->next;
    }
}

void C_conbine(struct Node **head_ref)
{
    struct Node *current = *head_ref;
    struct Node *perious = *head_ref;
    while (current != NULL && current->next != NULL)
    {
        perious = current;
        current = current->next;

        if (perious->process_id == -1 && current->process_id == -1)
        {
            combine_node(&perious, &current);
            current = perious;
        }
    }
}

void STAT_printList(struct Node **node)
{
    printf("\n");
    struct Node *current = *node;

    while (current != NULL)
    {
        printf("Addresses [%d:%d] ", current->start_address, current->end_address);
        // printf("Addresses [%d:%d] ", current->start_address, current->end_address);

        if (current->process_id == -1)
        {
            printf("Unused\n");
        }
        else
        {
            printf("Process P%d\n", current->process_id);
        }

        current = current->next;
    }
    printf("\n");
}

int main(int argc, char *argv[])
{

    struct Node *head;
    int inital_memory = atoi(argv[1]);
    init_memory(&head, inital_memory);

    while (1)
    {
        char user_input[128];

        char request[6];
        char process[1];
        char id[3];
        char avaliable[10];
        char way[3];

        printf("allocator >");
        fflush(stdout);

        if (read(0, user_input, 128) < 0)
        {
            write(2, "An error occurred in the read.\n", 31);
        }
        if ((sscanf(user_input, "%s %c%s %s %s", request, process, id, avaliable, way)) < 0)
        {
            write(2, "An error occurred in the read.\n", 31);
        }

        if (strcmp(request, "RQ") == 0)
        {
            if (strcmp(way, "F") == 0)
            {
                // F—first fit
                F_insert(&head, atoi(avaliable), atoi(id));
            }
            if (strcmp(way, "B") == 0)
            {
                // B—best fit
                B_insert(&head, atoi(avaliable), atoi(id));
            }
            if (strcmp(way, "W") == 0)
            {
                // W—worst fit
                W_insert(&head, atoi(avaliable), atoi(id));
            }
        }

        if (strcmp(request, "RL") == 0)
        {
            // release
            RL_release_node(&head, atoi(id));
        }

        if (strcmp(request, "C") == 0)
        {
            // compaction
            C_conbine(&head);
        }

        if (strcmp(request, "STAT") == 0)
        {
            // reporting the status
            STAT_printList(&head);
        }

        if (strcmp(request, "X") == 0)
        {
            break;
        }
        // STAT_printList(&head);
    }
}