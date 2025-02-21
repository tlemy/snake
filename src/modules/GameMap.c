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
            gm->grid[x * (gm->maxY) + y]         = calloc(1, sizeof(GridPosition));
            gm->grid[x * (gm->maxY) + y]->x       = x;
            gm->grid[x * (gm->maxY) + y]->y       = y;
            gm->grid[x * (gm->maxY) + y]->path    = 0;
            gm->grid[x * (gm->maxY) + y]->numHops = 0;
            gm->grid[x * (gm->maxY) + y]->type    = IS_FREE;
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
            gm->grid[x * (gm->maxY) + y]->path    = 0;
            gm->grid[x * (gm->maxY) + y]->numHops = 0;
            gm->grid[x * (gm->maxY) + y]->type    = IS_FREE;
        }
    }
}

void freeGameMap(GameMap* gm)
{
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

        if (parent.path == 0)
        {
            nearby->path = translateDirectionToKey(i);
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