#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "../include/Limit.h"

void initLimits(Limit* lim)
{
    lim->minX = 1;
    lim->minY = 2;

    getmaxyx(stdscr, lim->maxY, lim->maxX);
    lim->maxY -= 1;
    lim->maxX -= 1;

    /*
    * The squares being displayed take 1 row and 2 columns. ("  ")
    * As a consequence, the number of colums has to be pair.
    */
    if ((lim->maxX - 1) % 2 != 0)
    {
        lim->maxX -= 1;
    }

    lim->grid = (int*) malloc(lim->maxX * lim->maxY * sizeof(int));
}

void resetGrid(Limit* lim)
{
    memset(lim->grid, 0, lim->maxX * lim->maxY * (sizeof(int)));
}

void freeGrid(Limit* lim)
{
    free(lim->grid);
}

int isBorderCollision(Limit* lim, int x, int y)
{
    int leftBorderCollision = x < lim->minX;
    int topBorderCollision = y < lim->minY;
    int rightBorderCollision = x >= lim->maxX - 2; // each unit is 2 characters long
    int bottomBorderCollision = y >= lim->maxY - 1; // each unit is one character high

    return leftBorderCollision || rightBorderCollision || topBorderCollision || bottomBorderCollision;
}