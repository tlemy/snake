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

    for (int x = 0; x < gm->maxX; x++)
    {
        gm->grid[x] = (GridPosition*) malloc(gm->maxY * sizeof(GridPosition));

        for (int y = 0; y < gm->maxY; y++)
        {
            gm->grid[x][y].x = x;
            gm->grid[x][y].y = y;
            gm->grid[x][y].path = 0;
            gm->grid[x][y].numHops = 0;
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
            gm->grid[x][y].dir = 0;
            gm->grid[x][y].path = 0;
            gm->grid[x][y].numHops = 0;
            gm->grid[x][y].type = IS_FREE;
        }
    }
}

int freeGameMap(GameMap* gm)
{
    int x = 0;
    for (x = 0; x < gm->maxX; x++)
    {
        free(gm->grid[x]);
    }
    free(gm->grid);
    free(gm);

    return x;
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

int fetchNearby(GameMap* gm, GridPosition parent, GridPositionList* toVisit)
{
    int added = 0;

    for (int i = NORTH; i <= WEST; i++)
    {
        int x = parent.x;
        int y = parent.y;

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

        nearby->dir  = i;

        if (parent.path == 0)
        {
            nearby->path = translateDirectionToKey(nearby->dir);
        }
        else
        {
            nearby->path = parent.path;
        }

        nearby->numHops = parent.numHops + 1;

        if (nearby->type == IS_FREE || nearby->type == IS_APPLE)
        {
            toVisit = addElementToList(toVisit, nearby);
        }

        added++;
    }
    return added;
}

GridPositionList* fetchResults(GameMap* gm, GridPosition pos)
{
    GridPositionList* toVisit = newList();
    GridPositionList* results = newList();
    GridPositionElement* ptr  = toVisit->head;

    int maxHops = 10;
    int remain  = 1;
    pos.dir     = 0;

    addElementToList(toVisit, &pos);

    while (remain > 0)
    {
        if (ptr->pos->type == IS_APPLE)
        {
            results = addElementToList(results, ptr->pos);
        }
        else
        {
            ptr->pos->type = IS_VISITED;
            attron(COLOR_PAIR(BLACK_WHITE));
            mvaddstr(ptr->pos->y, ptr->pos->x, "  ");
            attroff(COLOR_PAIR(BLACK_WHITE));

            remain += fetchNearby(gm, *ptr->pos, toVisit);
        }

        remain -= 1;

        ptr = ptr->next;

        toVisit = removeFirstElementFromList(toVisit);

        if (ptr != NULL && ptr->pos->numHops >= maxHops)
        {
            break;
        }
    }

    freeList(toVisit);
    return results;
}

GridPosition* scan(GameMap* gm, int x, int y)
{
    GridPosition* pos = getGridPosition(gm, x, y);

    if (pos == NULL)
    {
        return NULL;
    }

    GridPositionList* results = fetchResults(gm, *pos);
    GridPositionElement* ptr = results->head;
    GridPosition* result;
    int minHops = -1;

    if (ptr->pos == NULL)
    {
        return NULL;
    }

    while (ptr != NULL)
    {
        if (ptr->pos->numHops < minHops || minHops == -1)
        {
            result = ptr->pos;
            minHops = result->numHops;
        }
        ptr = ptr->next;
    }

    return results->head->pos;
}

GridPositionElement* newListElement()
{
    GridPositionElement* element = (GridPositionElement*) malloc(sizeof(GridPositionElement));

    element->pos  = NULL;
    element->next = NULL;

    return element;
}

GridPositionList* newList()
{
    GridPositionList* list = (GridPositionList*) malloc(sizeof(GridPositionList));

    list->head = newListElement();
    list->tail = list->head;

    return list;
}

GridPositionList* addElementToList(GridPositionList* list, GridPosition* pos)
{
    if (list->head->pos == NULL)
    {
        list->head->pos = pos;
    }
    else
    {
        GridPositionElement* toAdd = newListElement();

        toAdd->pos = pos;

        list->tail->next = toAdd;

        list->tail = list->tail->next;
    }

    return list;
}

GridPositionList* removeFirstElementFromList(GridPositionList* list)
{
    if (list->head->next)
    {
        GridPositionElement* toFree = list->head;
        list->head = list->head->next;

        if (toFree)
        {
            free(toFree);
        }
    }
    return list;
}

void freeList(GridPositionList* list)
{
    GridPositionElement* ptr = list->head;

    while (ptr != NULL)
    {
        GridPositionElement* tmp = ptr;

        ptr = ptr->next;

        free(tmp);
    }
    free(list);
}