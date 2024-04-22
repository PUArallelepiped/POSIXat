#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 9
#define NUM_THREADS 27

int sudoku[SIZE][SIZE];
int valid[NUM_THREADS] = {0};

typedef struct {
    int id;
    int row;
    int col;
} parameters;

void checkRow(parameters *data){
    int row = data->row;
    int check[SIZE] = {0};
    for (int i = 0; i < SIZE; i++){
        if (check[sudoku[row][i] - 1] == 1){
            printf("Thread %d: Row %d is invalid\n", data->id, row);
            valid[data->id] = 0;
            return;
        }
        check[sudoku[row][i] - 1] = 1;
    }
    printf("Thread %d: Row %d is valid\n", data->id, row);
}

void checkCol(parameters *data){
    int col = data->col;
    int check[SIZE] = {0};
    for (int i = 0; i < SIZE; i++){
        if (check[sudoku[i][col] - 1] == 1){
            printf("Thread %d: Col %d is invalid\n", data->id, col);
            valid[data->id] = 0;
            return;
        }
        check[sudoku[i][col] - 1] = 1;
    }
    printf("Thread %d: Col %d is valid\n", data->id, col);
}

void checkSquare(parameters *data){
    int row = data->row;
    int col = data->col;
    int id = data->id;
    int check[SIZE] = {0};
    for (int i = row; i < row + 3; i++){
        for (int j = col; j < col + 3; j++){
            if (check[sudoku[i][j] - 1] == 1){
                printf("Thread %d: Square %d %d is invalid\n", data->id, row, col);
                valid[id] = 0;
                return;
            }
            check[sudoku[i][j] - 1] = 1;
        }
    }
    printf("Thread %d: Square %d %d is valid\n", data->id, row, col);
}

int main (int argc, char *argv[]){
    char *filename;
    if (argc != 2){
        printf("Usage: ./sudoku <sudoku.csv>\n");
        return 0;
    }
    else {
        filename = argv[1];
    }

    // initialize sudoku with file input
    FILE *file = fopen(filename, "r");
    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
            fscanf(file, "%d,", &sudoku[i][j]);
        }
    }
    fclose(file);

    // initialize valid array
    for (int i = 0; i < NUM_THREADS; i++){
        valid[i] = 1;
    }

    pthread_t threads[NUM_THREADS];
    parameters data[NUM_THREADS] = {0};
    int id = 0;

    // check rows
    for (int i = 0; i < SIZE; i++){
        data[id].row = i;
        data[id].id = id;
        pthread_create(&threads[id], NULL, (void *) checkRow, (void *)(data + id));
        id += 1;
    }

    // check cols
    for (int i = 0; i < SIZE; i++){
        data[id].col = i;
        data[id].id = id;
        pthread_create(&threads[id], NULL, (void *) checkCol, (void *)(data + id));
        id += 1;
    }

    // check squares
    for (int i = 0; i < SIZE; i += 3){
        for (int j = 0; j < SIZE; j += 3){
            data[id].row = i;
            data[id].col = j;
            data[id].id = id;
            pthread_create(&threads[id], NULL, (void *) checkSquare, (void *)(data + id));
            id += 1;
        }
    }    

    for (int i = 0; i < NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    // check if sudoku is valid
    for (int i = 0; i < NUM_THREADS; i++){
        if (valid[i] == 0){
            printf("Sudoku is invalid\n");
            return 0;
        }
    }
    printf("Sudoku is valid\n");

    return 0;
}