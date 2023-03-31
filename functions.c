#ifndef FUNCTIONS_C
#define FUNCTIONS_C

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include "types.h"

int isCollision(struct Position **snakeArray, struct Position *position, int snakeSize);
void displayGameArray(char* gameArray, int gameSize);
void clear_screen();
char* createGameArray(int gameSize);
void resetGameArray(char *gameArray, int gameSize);
void insertSnakeToGame(struct Position *snakeArray, char *gameArray, int gameSize, int snakeSize);
int updateSnakeArray(struct Position **snakeArray, int gameSize, int inputState, int *snakeSize, struct Position *apple);
void increaseSnakeLength(struct Position **snakeArray, int *snakeSize);
void addApple(char **gameArray, struct Position apple, int gameSize);
struct Position getRandApplePosition(struct Position **snakeArray, struct Position *currentApple, int gameSize, int snakeSize);
int isFar(struct Position *currentApple, struct Position *newApple, int gameSize);

void increaseSnakeLength(struct Position **snakeArray, int *snakeSize) {
    *snakeSize = *snakeSize + 1;
    *snakeArray = (struct Position*) realloc(*snakeArray, *snakeSize * sizeof(struct Position));
}


int updateSnakeArray(struct Position **snakeArray, int gameSize, int inputState, int *snakeSize, struct Position *apple) {
    struct Position snakeHead = (*snakeArray)[0];

    if (inputState == UP) {
        snakeHead.y--;
    } else if (inputState == RIGHT) {
        snakeHead.x++;
    }
    else if (inputState == DOWN) {
        snakeHead.y++;
    } else if (inputState == LEFT) {
        snakeHead.x--;
    }

    if (snakeHead.x > gameSize - 2) {
        snakeHead.x = 1;
    } else if (snakeHead.x == 0) {
        snakeHead.x = gameSize - 2;
    } else if (snakeHead.y > gameSize - 2) {
        snakeHead.y = 1;
    } else if (snakeHead.y == 0) {
        snakeHead.y = gameSize - 2;
    }

    if (isCollision(snakeArray, &snakeHead, *snakeSize)) {
        return 0;
    }

    if (snakeHead.x == apple->x && snakeHead.y == apple->y) {
        *apple = getRandApplePosition(snakeArray, apple, gameSize, *snakeSize);
        increaseSnakeLength(snakeArray, snakeSize);
    }

    for (int i = *snakeSize - 1; i > 0; i--) {
        (*snakeArray)[i].x = (*snakeArray)[i - 1].x;
        (*snakeArray)[i].y = (*snakeArray)[i - 1].y;
    }


    (*snakeArray)[0] = snakeHead;

    return 1;
}

int isCollision(struct Position** snakeArray, struct Position* position, int snakeSize) {
    for (int i = 0; i < snakeSize; ++i) {
        if (position->x == (*snakeArray)[i].x && position->y == (*snakeArray)[i].y) {
            return 1;
        }
    }
    return 0;
}

void insertSnakeToGame(struct Position *snakeArray, char *gameArray, int gameSize, int snakeSize) {
    resetGameArray(gameArray, gameSize);
    for (int i = 0; i < gameSize; i++) {
        for (int j = 0; j < gameSize; j++) {
            for (int k = 0; k < snakeSize; k++) {
                if (snakeArray[k].x == i && snakeArray[k].y == j) {
                    *(gameArray + i + gameSize * j) = '*';
                }
            }
        }
    }
}

char *createGameArray(int gameSize) {
    char *arr = (char *)malloc(gameSize * gameSize * sizeof(char));
    resetGameArray(arr, gameSize);
    return arr;
}

void resetGameArray(char *gameArray, int gameSize) {
    int i, j;
    for (i = 0; i < gameSize; i++)
        for (j = 0; j < gameSize; j++)
            if (i == 0 || i == gameSize - 1 || j == 0 || j == gameSize - 1)
                *(gameArray + i * gameSize + j) = '#';
            else
                *(gameArray + i * gameSize + j) = ' ';
}


void displayGameArray(char *gameArray, int gameSize) {
    for (int i = 0; i < gameSize; i++) {
        for (int j = 0; j < gameSize; j++)
            printf("%c ", *(gameArray + i * gameSize + j));

        printf("\n");
    }
}

void addApple(char **gameArray, struct Position apple, int gameSize) {
    *(*gameArray + apple.y * gameSize + apple.x) = '$';
}

struct Position getRandApplePosition(struct Position **snakeArray, struct Position *currentApple, int gameSize, int snakeSize) {
    struct Position* availPositions = malloc(sizeof(struct Position) * gameSize * gameSize);
    int length = 0;
    for (int i = 1; i < gameSize - 1; ++i) {
        for (int j = 1; j < gameSize - 1; ++j) {
            struct Position newApple = {j, i};
            if (!isCollision(snakeArray, &newApple, snakeSize)  && isFar(currentApple, &newApple, gameSize)) {
                availPositions[length] = (struct Position) {j, i};
                length++;
            }
        }
    }
    srand(time(NULL));
    return availPositions[rand() % length];
}

int isFar(struct Position *currentApple, struct Position *newApple, int gameSize) {
    int space = gameSize / 3;
    if (currentApple->x + space >  newApple->x && currentApple->x - space < newApple->x &&
        currentApple->y + space >  newApple->y && currentApple->y - space < newApple->y)
        return 0;
    return 1;
}

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
#endif
}

#endif