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

void hole_node(struct Node **new_node_ref, int _available_space, int start_address)
{
    struct Node *new_node = *new_node_ref;
    new_node->available_space = _available_space;
    new_node->process_id = -1;
    new_node->next = NULL;

    new_node->start_address = start_address;
    new_node->end_address = start_address + _available_space - 1;
    return;
}

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

void conbine(struct Node **head_ref)
{
    struct Node *current = *head_ref;
    struct Node *previous = *head_ref;
    while (current != NULL && current->next != NULL)
    {
        previous = current;
        current = current->next;

        if (previous->process_id == -1 && current->process_id == -1)
        {
            combine_node(&previous, &current);
            current = previous;
        }
    }
}

void F_insert(struct Node **head_ref, int _available_space, int _process_id)
{
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));

    new_node->available_space = _available_space;
    new_node->process_id = _process_id;
    new_node->next = NULL;

    struct Node *current = *head_ref;
    struct Node *previous = *head_ref;

    while (current != NULL)
    {

        if ((current->process_id == -1) && (new_node->available_space < current->available_space))
        {
            if (previous == current && previous == *head_ref)
            {
                sep_node(head_ref, &current, &new_node); // if is head
            }
            else
            {
                sep_node(&previous, &current, &new_node);
            };
            return;
        }
        else if ((current->process_id == -1) && (new_node->available_space == current->available_space))
        {
            current->process_id = _process_id;
            free(new_node);

            return;
        }
        previous = current;
        current = current->next;
    }

    printf("no memory big enough to use\n");
}

void B_insert(struct Node **head_ref, int _available_space, int _process_id)
{
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));

    new_node->available_space = _available_space;
    new_node->process_id = _process_id;
    new_node->next = NULL;

    struct Node *current = *head_ref;
    struct Node *previous = *head_ref;
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
                best_pre = previous;
                best = current;
                diff = _diff;
            }
        }
        previous = current;
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
    else
    {
        printf("no memory big enough to use\n");
    }
}

void W_insert(struct Node **head_ref, int _available_space, int _process_id)
{
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));

    new_node->available_space = _available_space;
    new_node->process_id = _process_id;
    new_node->next = NULL;

    struct Node *current = *head_ref;
    struct Node *previous = *head_ref;
    struct Node *worst = NULL;
    struct Node *worst_pre = NULL;
    int MAX = -1;
    while (current != NULL)
    {

        if ((current->process_id == -1) && (new_node->available_space <= current->available_space))
        {
            int _diff = current->available_space - new_node->available_space;
            if ((_diff > MAX))
            {
                worst_pre = previous;
                worst = current;
                MAX = _diff;
            }
        }

        previous = current;
        current = current->next;
    }

    if (worst != NULL)
    {
        if (MAX == 0)
        {
            worst->process_id = _process_id;
            free(new_node);
            return;
        }
        if ((worst_pre == worst) && (worst_pre == *head_ref))
        {
            sep_node(head_ref, &worst, &new_node); // if is head
            return;
        }

        sep_node(&worst_pre, &worst, &new_node);
    }
    else
    {
        printf("no memory big enough to use\n");
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
            conbine(head_ref);
            return;
        }
        current = current->next;
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

void C_compaction(struct Node **head_ref)
{
    struct Node *current = *head_ref;
    struct Node *previous = *head_ref;
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));

    int count = 0;

    while (current != NULL)
    {

        if ((current->process_id == -1))
        {
            count++;
        }
        current = current->next;
    }
    if (count <= 1)
    {
        return;
    }

    int all_hole = 0;
    current = *head_ref; // reset to head

    while (current != NULL)
    {

        if ((current->process_id == -1))
        {
            all_hole += current->available_space;

            if (previous == current && previous == *head_ref)
            {
                *head_ref = current->next;
            }
            else
            {
                previous->next = current->next;
            }

            struct Node *rellocate = current->next;

            while (rellocate != NULL)
            {

                rellocate->start_address -= current->available_space;
                rellocate->end_address -= current->available_space;
                rellocate = rellocate->next;
            }

            free(current);
            current = previous;
        }
        previous = current;
        current = current->next;
    }

    hole_node(&new_node, all_hole, previous->end_address + 1);
    previous->next = new_node;
}

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <memory size>\n", argv[0]);
        return 1;
    }

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
            C_compaction(&head);
        }

        if (strcmp(request, "STAT") == 0)
        {
            // reporting the status
            STAT_printList(&head);
        }

        if (strcmp(request, "X") == 0)
        {
            // exit
            break;
        }
        // STAT_printList(&head);
    }
}