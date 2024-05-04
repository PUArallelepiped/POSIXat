#include <stdio.h>

typedef int buffer_item;

#define BUFFER_SIZE 5

buffer_item buffer[BUFFER_SIZE];

int head = 0;
int tail = 0;

void print_buffer() {
    printf("☆.。.:*・");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        printf("%10d ", buffer[i]);
    }
    printf("・*:。.☆");
    printf("\n");
}

/*  insert item into buffer 
    return 0 if successful, otherwise
    return -1 indicating an error condition */
int insert_item(buffer_item item) {
    if ((tail + 1) % BUFFER_SIZE == head) {
        return -1;
    }
    buffer[tail] = item;
    tail = (tail + 1) % BUFFER_SIZE;
    print_buffer();
    return 0;
}

/*  remove an object from buffer placing it in item
    return 0 if successful, otherwise
    return -1 indicating an error condition */
int remove_item(buffer_item *item) {
    if (head == tail) {
        return -1;
    }
    *item = buffer[head];
    buffer[head] = 0;
    head = (head + 1) % BUFFER_SIZE;
    print_buffer();
    return 0;
}

void init_buffer() {
    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer[i] = 0;
    }
}
