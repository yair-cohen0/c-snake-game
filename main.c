#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <stdint.h>
#include <conio.h>


struct Position {
    int x;
    int y;
};

void displayArr(char* arr, int size);
void clear_screen();
char* createArray(int size);
void *insertSnakeToArr(struct Position *snake, char **arr, int size, int snakeSize);
void updateSnakeArr(struct Position *snake, int size, int state, int snakeSize);
void resetSnake(struct Position* snake, int size);
void increaseSnakeLength(struct Position** snake, int* snakeSize);


int main() {
    char lastChar;
    int size = 8;
    int state = 2;
    clock_t startTime, currTime;
    double diff;
    int snakeSize = 1;
    int tick = 0;

    struct Position *snake = malloc(snakeSize * sizeof(struct Position));
    snake[0] = (struct Position){5, 1};

    char* arr = createArray(size);

    displayArr(arr, size);
    startTime = clock();
    while (1) {
        currTime = clock();
        diff = ((double) currTime - startTime) / CLOCKS_PER_SEC;
        if (diff > 0.3) {
            startTime = clock();
            clear_screen();

            tick++;
            if (tick == 5) {
                increaseSnakeLength(&snake, &snakeSize);
                tick = 0;
            }

            updateSnakeArr(snake, size, state, snakeSize);
            insertSnakeToArr(snake, &arr, size, snakeSize);
            displayArr(arr, size);
        }

        if (kbhit()) {
            lastChar = getch();
            if (lastChar == 'a'){
                state = 4;
            } else if (lastChar == 'd') {
                state = 2;
            } else if (lastChar == 's') {
                state = 3;
            } else if (lastChar == 'w') {
                state = 1;
            } else if (lastChar == 'q') {
                free(snake);
                free(arr);
                return 1;
            }
        }
    }

    free(arr);
    return 0;
}

void increaseSnakeLength(struct Position** snake, int* snakeSize) {
    *snake = (struct Position*) realloc(*snake, (*snakeSize + 1) * sizeof(struct Position));
    *snakeSize = *snakeSize + 1;
}


void updateSnakeArr(struct Position *snake, int size, int state, int snakeSize) {

    for (int i = snakeSize - 1; i > 0; i--) {
        snake[i].x = snake[i-1].x;
        snake[i].y = snake[i-1].y;
    }

    if (state == 1) {
        snake[0].y--;
    } else if (state == 2) {
        snake[0].x++;
    }
    else if (state == 3) {
        snake[0].y++;
    } else if (state == 4) {
        snake[0].x--;
    }

    if (snake[0].x > size - 2) {
        snake[0].x = 1;
    } else if (snake[0].x == 0) {
        snake[0].x = size - 2;
    } else if (snake[0].y > size - 2) {
        snake[0].y = 1;
    } else if (snake[0].y == 0) {
        snake[0].y = size - 2;
    }
}

void *insertSnakeToArr(struct Position *snake, char **arr, int size, int snakeSize) {
    free(*arr);
    *arr = createArray(size);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < snakeSize; k++) {
                if (snake[k].x == 0 && snake[k].y == 0) break;

                if (snake[k].x == i && snake[k].y == j) {
                    *(*arr + i + size * j) = '*';
                }
            }
        }
    }
}

char *createArray(int size) {
    char *arr = (char *)malloc(size * size * sizeof(char));
    int i, j;
    for (i = 0; i < size; i++)
        for (j = 0; j < size; j++)
            if (i == 0 || i == size - 1 || j == 0 || j == size - 1)
                *(arr + i*size + j) = '*';
            else
                *(arr + i*size + j) = ' ';

    return arr;
}


void displayArr(char *arr, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++)
            printf("%c ", *(arr + i*size + j));

        printf("\n");
    }
}

void resetSnake(struct Position* snake, int size) {
    snake[0] = (struct Position){3,1};
    for (int i = 1; i < size * size; i++) {
        snake[i] = (struct Position){0,0};;
    }
    snake[1] = (struct Position){2,1};
    snake[2] = (struct Position){1,1};
}

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
#endif
}