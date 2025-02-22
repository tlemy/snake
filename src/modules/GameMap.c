#include "GameMap.h"
#include "Incrementation.h"
#include "Direction.h"
#include "Key.h"
#include "Color.h"

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define SPACE ' '

GameMap* newGameMap(int minX, int minY, int maxX, int maxY)
{
    GameMap* gm = calloc(1, sizeof(GameMap));

    if (!gm)
    {
        perror("newGameMap");
        exit(-1);
    }

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

    gm->grid = calloc(gm->maxX * gm->maxY, sizeof(GridPosition**));

    if (!gm->grid)
    {
        perror("newGameMap");
        exit(-1);
    }

    for (int x = 0; x < gm->maxX; x++)
    {
        for (int y = 0; y < gm->maxY; y++)
        {
            gm->grid[x * (gm->maxY) + y]          = calloc(1, sizeof(GridPosition));
            gm->grid[x * (gm->maxY) + y]->x       = x;
            gm->grid[x * (gm->maxY) + y]->y       = y;
            gm->grid[x * (gm->maxY) + y]->path    = 0;
            gm->grid[x * (gm->maxY) + y]->numHops = 0;
            gm->grid[x * (gm->maxY) + y]->type    = IS_FREE;
            gm->grid[x * (gm->maxY) + y]->dir     = 0;
        }
    }

    return gm;
}

void resetGridGameMap(GameMap* gm)
{
    for (int x = 0; x < gm->maxX; x++)
    {
        for (int y = 0; y < gm->maxY; y++)
        {
            gm->grid[x * (gm->maxY) + y]->x       = x;
            gm->grid[x * (gm->maxY) + y]->y       = y;
            gm->grid[x * (gm->maxY) + y]->path    = 0;
            gm->grid[x * (gm->maxY) + y]->numHops = 0;
            gm->grid[x * (gm->maxY) + y]->type    = IS_FREE;
        }
    }
}

void freeGameMap(GameMap* gm)
{
    for (int x = 0; x < gm->maxX; x++)
    {
        for (int y = 0; y < gm->maxY; y++)
        {
            free(gm->grid[x * (gm->maxY) + y]);
        }
    }
    free(gm->grid);
    free(gm);
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
    if (!gm)
    {
        return NULL;
    }

    if (x >= gm->maxX || x < gm->minX)
    {
        return NULL;
    }

    if (y >= gm->maxY || y < gm->minY)
    {
        return NULL;
    }

    return gm->grid[x * (gm->maxY) + y];
}

void setGridPosition(GameMap* gm, int x, int y, int type)
{
    GridPosition* pos = getGridPosition(gm, x, y);

    if (pos)
    {
         pos->type = type;
    }
}

CoordinateList* addElementToList(CoordinateList* list, GridPosition* pos)
{
    if (list->idxAdd < MAX_LEN) // TODO: why adding more than max len
    {
        list->arr[list->idxAdd].x = pos->x;
        list->arr[list->idxAdd].y = pos->y;

        list->idxAdd += 1;
    }

    return list;
}

int fetchNearby(GameMap* gm, GridPosition* parent, CoordinateList* toVisit)
{
    int added = 0;

    for (int i = NORTH; i <= WEST; i++)
    {
        int x = parent->x;
        int y = parent->y;

        switch (i)
        {
        case NORTH:
            y -= Y_INC_SNAKE;
            break;
        case SOUTH:
            y += Y_INC_SNAKE;
            break;
        case EAST:
            x += X_INC_SNAKE;
            break;
        case WEST:
            x -= X_INC_SNAKE;
            break;
        default:
            x = -1;
            y = -1;
        }

        GridPosition* nearby = getGridPosition(gm, x, y);

        if (nearby == NULL)
        {
            continue;
        }

        if (nearby->type == IS_VISITED || nearby->type == IS_SNAKE)
        {
            continue;
        }

        if (parent->path == 0)
        {
            nearby->path = translateDirectionToKey(i);
        }
        else
        {
            nearby->path = parent->path;
        }

        nearby->numHops = parent->numHops + 1;
        nearby->dir     = i;

        if (nearby->type == IS_FREE || nearby->type == IS_APPLE)
        {
            toVisit = addElementToList(toVisit, nearby);
        }

        added++;
    }
    return added;
}

void fetchResults(GameMap* gm, GridPosition* initPos, CoordinateList* results)
{
    CoordinateList toVisit;

    for (int i = 0; i < MAX_LEN; i++)
    {
        toVisit.arr->x = 0;
        toVisit.arr->y = 0;
    }

    toVisit.idxAdd = 0;

    addElementToList(&toVisit, initPos);

    for (int i = 0; i < toVisit.idxAdd; i++)
    {
        Coordinate coor   = toVisit.arr[i];
        GridPosition* pos = getGridPosition(gm, coor.x, coor.y);

        if (!pos) // why random x or y value
        {
            continue;
        }

        if (toVisit.idxAdd < MAX_LEN && pos->numHops >= MAX_HOPS)
        {
            break;
        }

        if (pos->type == IS_APPLE)
        {
            results = addElementToList(results, pos);
        }
        else
        {
            setGridPosition(gm, coor.x, coor.y, IS_VISITED);

            attron(COLOR_PAIR(BLACK_WHITE));
            mvaddstr(pos->y, pos->x, "  ");
            attroff(COLOR_PAIR(BLACK_WHITE));

            fetchNearby(gm, pos, &toVisit);
        }
    }
}

GridPosition* scan(GameMap* gm, int x, int y)
{
    GridPosition* pos = getGridPosition(gm, x, y);

    if (pos == NULL)
    {
        return NULL;
    }

    CoordinateList results;

    for (int i = 0; i < MAX_LEN; i++)
    {
        results.arr->x = 0;
        results.arr->y = 0;
    }

    results.idxAdd = 0;

    fetchResults(gm, pos, &results);

    int result  = -1;
    int minHops = -1;

    for (int i = 0; i < results.idxAdd; i++)
    {
        Coordinate coor   = results.arr[i];
        GridPosition* pos = getGridPosition(gm, coor.x, coor.y);

        if (pos->numHops < minHops || minHops == -1)
        {
            result = i;
            minHops = pos->numHops;
        }
    }

    if (result >= 0)
    {
        Coordinate coor = results.arr[result];

        return getGridPosition(gm, coor.x, coor.y);
    }

    return NULL;
}