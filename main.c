#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include "functions.c"
#include "types.h"


int main() {
    char lastChar;
    enum State inputState = RIGHT;

    double diff;
    clock_t startTime, currTime;


    int gameSize = 10;
    int snakeSize = 1;



    struct Position *snakeArr = malloc(snakeSize * sizeof(struct Position));
    snakeArr[0] = (struct Position){1, 1};

    struct Position apple = getRandApplePosition(&snakeArr, &snakeArr[0], gameSize, snakeSize);

    char* gameArray = createGameArray(gameSize);
    addApple(&gameArray, apple, gameSize);

    displayGameArray(gameArray, gameSize);
    startTime = clock();
    while (1) {
        currTime = clock();
        diff = ((double) currTime - startTime) / CLOCKS_PER_SEC;
        if (diff > 0.3) {
            startTime = clock();
            clear_screen();

            if (!updateSnakeArray(&snakeArr, gameSize, inputState, &snakeSize, &apple)) {
                printf("YOU LOST!\n\nPress q to exit or r to restart");
                while (1) {
                    lastChar = getch();
                    if (lastChar == 'q')
                        return 0;
                    else if (lastChar == 'r') {
                        free(snakeArr);
                        free(gameArray);
                        clear_screen();
                        return main();
                    }
                }
            }
            insertSnakeToGame(snakeArr, gameArray, gameSize, snakeSize);
            addApple(&gameArray, apple, gameSize);
            displayGameArray(gameArray, gameSize);
        }

        if (kbhit()) {
            lastChar = (char)getch();
            if (lastChar == 'a'){
                inputState = LEFT;
            } else if (lastChar == 'd') {
                inputState = RIGHT;
            } else if (lastChar == 's') {
                inputState = DOWN;
            } else if (lastChar == 'w') {
                inputState = UP;
            } else if (lastChar == 'q') {
                free(snakeArr);
                free(gameArray);
                return 1;
            }
        }
    }

    free(gameArray);
    return 0;
}