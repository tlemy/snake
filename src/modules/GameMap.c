#include "GameMap.h"

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define SPACE ' '

GameMap* newGameMap(int minX, int minY, int maxX, int maxY)
{
    GameMap* gm = (GameMap*) malloc(sizeof(GameMap));

    gm->minX = 1;
    gm->minY = 2;
    gm->maxX = maxX - 1;
    gm->maxY = maxY - 1;

    /*
    * The squares being displayed take 1 row and 2 columns. ("  ")
    * As a consequence, the number of colums has to be pair.
    */
    if ((gm->maxX - 1) % 2 != 0)
    {
        gm->maxX -= 1;
    }

    gm->grid = (int**) malloc(gm->maxX * sizeof(int*));

    for (int i = 0; i < gm->maxX; i++)
    {
        gm->grid[i] = (int*) malloc(gm->maxY * sizeof(int));
    }

    return gm;
}

void resetGridGameMap(GameMap* gm)
{
    for (int i = 0; i < gm->maxX; i++)
    {
        memset(gm->grid[i], 0, gm->maxY * (sizeof(int)));
    }
}

int freeGameMap(GameMap* gm)
{
    int i = 0;
    for (i = 0; i < gm->maxX; i++)
    {
        free(gm->grid[i]);
    }
    free(gm->grid);
    free(gm);

    return i;
}

int isBorderCollision(GameMap* gm, int x, int y)
{
    int leftBorderCollision   = x < gm->minX;
    int topBorderCollision    = y < gm->minY;
    int rightBorderCollision  = x >= gm->maxX - 2; // each unit is 2 characters long
    int bottomBorderCollision = y >= gm->maxY - 1; // each unit is one character high

    return leftBorderCollision || rightBorderCollision || topBorderCollision || bottomBorderCollision;
}

void drawBorders(int maxX, int maxY, int col, int score)
{
    char str[32];
    sprintf(str, " [ SCORE: %d ] ", score);
    attron(COLOR_PAIR(col));
    mvaddstr(0, 1, str);
    attroff(COLOR_PAIR(col));

    attron(COLOR_PAIR(col));

    // top
    for (int x = strlen(str) + 1; x <= maxX ; x++)
    {
        mvaddch(0, x, SPACE);
    }

    // bottom
    for (int x = 0; x <= maxX; x++)
    {
        mvaddch(maxY, x, SPACE);
    }

    // left
    for (int y = 0; y < maxY; y++)
    {
        mvaddch(y, 0, SPACE);
    }

    // right
    for (int y = 0; y < maxY; y++)
    {
        mvaddch(y, maxX, SPACE);
    }

    attroff(COLOR_PAIR(col));
}

PostionType getAtGameMapPosition(GameMap* gm, int x, int y)
{
    return gm->grid[x][y];
}

int setAtGameMapPosition(GameMap* gm, int x, int y, PostionType type)
{
    if (x >= gm->maxX || x < gm->minX)
    {
        return -1;
    }
    if (y >= gm->maxY || y < gm->minY)
    {
        return -1;
    }

    gm->grid[x][y] = type;
    return 0;
}