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
void resetArray(char *arr, int size);
void insertSnakeToArr(struct Position *snake, char *arr, int size, int snakeSize);
void updateSnakeArr(struct Position **snake, int size, int state, int *snakeSize, struct Position *apple);
void resetSnake(struct Position* snake, int size);
void increaseSnakeLength(struct Position **snake, int *snakeSize);
void addApple(char **arr, struct Position pos, int size);
struct Position getRandApplePosition(struct Position snakeHead, int size);


int main() {
    char lastChar;
    int size = 18;
    int state = 2;
    clock_t startTime, currTime;
    double diff;
    int snakeSize = 1;
    int tick = 0;

    struct Position *snake = malloc(snakeSize * sizeof(struct Position));
    snake[0] = (struct Position){5, 1};

    struct Position apple = getRandApplePosition(snake[0], size);

    char* arr = createArray(size);
    addApple(&arr, apple, size);

    displayArr(arr, size);
    startTime = clock();
    while (1) {
        currTime = clock();
        diff = ((double) currTime - startTime) / CLOCKS_PER_SEC;
        if (diff > 0.3) {
            startTime = clock();
            clear_screen();

            updateSnakeArr(&snake, size, state, &snakeSize, &apple);

            insertSnakeToArr(snake, arr, size, snakeSize);
            addApple(&arr, apple, size);
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

void increaseSnakeLength(struct Position **snake, int *snakeSize) {
    *snakeSize = *snakeSize + 1;
    *snake = (struct Position*) realloc(*snake, *snakeSize * sizeof(struct Position));
}


void updateSnakeArr(struct Position **snake, int size, int state, int *snakeSize, struct Position *apple) {

    struct Position tempHead = (*snake)[0];

    if (state == 1) {
        tempHead.y--;
    } else if (state == 2) {
        tempHead.x++;
    }
    else if (state == 3) {
        tempHead.y++;
    } else if (state == 4) {
        tempHead.x--;
    }

    if (tempHead.x > size - 2) {
        tempHead.x = 1;
    } else if (tempHead.x == 0) {
        tempHead.x = size - 2;
    } else if (tempHead.y > size - 2) {
        tempHead.y = 1;
    } else if (tempHead.y == 0) {
        tempHead.y = size - 2;
    }

    if (tempHead.x == apple->x && tempHead.y == apple->y) {
        *apple = getRandApplePosition((*snake)[0], size);
        increaseSnakeLength(snake, snakeSize);
    }

    for (int i = *snakeSize - 1; i > 0; i--) {
        (*snake)[i].x = (*snake)[i-1].x;
        (*snake)[i].y = (*snake)[i-1].y;
    }


    (*snake)[0] = tempHead;

}

void insertSnakeToArr(struct Position *snake, char *arr, int size, int snakeSize) {
    resetArray(arr, size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < snakeSize; k++) {
                if (snake[k].x == i && snake[k].y == j) {
                    *(arr + i + size * j) = '*';
                }
            }
        }
    }
}

char *createArray(int size) {
    char *arr = (char *)malloc(size * size * sizeof(char));
    resetArray(arr, size);
    return arr;
}

void resetArray(char *arr, int size) {
    int i, j;
    for (i = 0; i < size; i++)
        for (j = 0; j < size; j++)
            if (i == 0 || i == size - 1 || j == 0 || j == size - 1)
                *(arr + i*size + j) = '*';
            else
                *(arr + i*size + j) = ' ';
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

int getRandomInt(int max) {
}

void addApple(char **arr, struct Position pos, int size) {
    *(*arr + pos.y * size + pos.x) = '$';
}

struct Position getRandApplePosition(struct Position snakeHead, int size) {
    srand(time(NULL));
    struct Position pos;
    do {
        pos = (struct Position){rand() % size, rand() % size};
    } while (pos.x <= 1 || pos.y <= 1 || pos.x >= size - 1 || pos.y >= size - 1);
    return pos;
}

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
#endif
}