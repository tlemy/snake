#include "GameMap.h"
#include "Incrementation.h"
#include "Direction.h"

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

    gm->grid = (GridPosition**) malloc(gm->maxX * sizeof(GridPosition*));

    for (int i = 0; i < gm->maxX; i++)
    {
        gm->grid[i] = (GridPosition*) malloc(gm->maxY * sizeof(GridPosition));
    }

    return gm;
}

void resetGridGameMap(GameMap* gm)
{
    for (int i = 0; i < gm->maxX; i++)
    {
        memset(gm->grid[i], 0, gm->maxY * (sizeof(GridPosition)));
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

GridPosition* getGridPosition(GameMap* gm, int x, int y)
{
    if (x >= gm->maxX || x < gm->minX)
    {
        return NULL;
    }
    if (y >= gm->maxY || y < gm->minY)
    {
        return NULL;
    }

    GridPosition* pos = &(gm->grid[x][y]);
    pos->x = x;
    pos->y = y;

    return &(gm->grid[x][y]);
}

GridPosition* setGridPosition(GameMap* gm, int x, int y, int type)
{
    GridPosition* pos = getGridPosition(gm, x, y);

    if (!pos)
    {
        return pos;
    }

    pos->type = type;

    return pos;
}

GridPosition* fetch(GameMap* gm, GridPosition* ptr)
{
    for (int i = 1; i < 5; i++) // directions
    {
        GridPosition* pos = NULL;

        switch (i)
        {
        case NORTH:
            pos = getGridPosition(gm, ptr->x, ptr->y - Y_INC_SNAKE);
            break;
        case SOUTH:
            pos = getGridPosition(gm, ptr->x, ptr->y + Y_INC_SNAKE);
            break;
        case WEST:
            pos = getGridPosition(gm, ptr->x - X_INC_SNAKE, ptr->y);
            break;
        case EAST:
            pos = getGridPosition(gm, ptr->x + X_INC_SNAKE, ptr->y);
            break;
        default:
            break;
        }

        GridPosition* result = NULL;

        if (pos == NULL)
        {
            return NULL;
        }
        else if (pos->type == IS_APPLE)
        {
            result = pos;
        }
        else if (pos != NULL && pos->type == IS_FREE)
        {
            result = fetch(gm, pos);
        }

        pos->parentX = ptr->x;
        pos->parentY = ptr->y;
        pos->dir = i;
        pos->type = IS_VISITED;

        return result;
    }
}

GridPosition* scan(GameMap* gm, int x, int y)
{
    return fetch(gm, &(gm->grid[x][y]));
}