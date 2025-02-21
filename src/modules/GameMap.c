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

    gm->grid = calloc(gm->maxX * gm->maxY, sizeof(GridPosition*));

    if (!gm->grid)
    {
        perror("newGameMap");
        exit(-1);
    }

    for (int x = 0; x < gm->maxX; x++)
    {
        for (int y = 0; y < gm->maxY; y++)
        {
            GridPosition *pos = calloc(1, sizeof(GridPosition));

            if (!pos)
            {
                perror("newGameMap");
                exit(-1);
            }

            pos->x       = x;
            pos->y       = y;
            pos->path    = 0;
            pos->numHops = 0;
            pos->type    = IS_FREE;

            gm->grid[x * (gm->maxY) + y] = pos;
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
            GridPosition *pos = gm->grid[x * (gm->maxY) + y];

            if (!pos)
            {
                perror("resetGridGameMap");
                exit(-1);
            }

            pos->path    = 0;
            pos->numHops = 0;
            pos->type    = IS_FREE;
        }
    }
    gm->idxResults = 0;
    gm->idxToVisit = 0;
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

int fetchNearby(GameMap* gm, GridPosition* parent)
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

        if (nearby && (nearby->type == IS_FREE || nearby->type == IS_APPLE))
        {
            nearby->numHops = parent->numHops + 1;

            if (parent->path == 0)
            {
                nearby->path = translateDirectionToKey(i);
            }
            else
            {
                nearby->path = parent->path;
            }

            if (EXIT_FAILURE == addToVisit(gm, nearby))
            {
                // perror("fetchNeaby");
                // exit(EXIT_FAILURE);
            }
            else
            {
                added += 1;
            }
        }
    }
    return added;
}

void fetchResults(GameMap* gm, GridPosition* initPos)
{
    int maxHops = 10;
    int remain  = 1;

    addToVisit(gm, initPos);

    int i = 0;

    while (remain > 0)
    {
        Coordinate coor   = gm->toVisit[i];
        GridPosition* pos = getGridPosition(gm, coor.x, coor.y);

        if (!pos)
        {
            continue;
        }

        if (pos->type == IS_APPLE)
        {
            addResults(gm, pos);
        }
        else
        {
            pos->type = IS_VISITED;
            attron(COLOR_PAIR(BLACK_WHITE));
            mvaddstr(pos->y, pos->x, "  ");
            attroff(COLOR_PAIR(BLACK_WHITE));

            remain += fetchNearby(gm, pos);
        }

        remain -= 1;

        if (pos->numHops >= maxHops)
        {
            break;
        }

        i++;
    }
}

GridPosition* scan(GameMap* gm, int x, int y)
{
    GridPosition* initPos = getGridPosition(gm, x, y);

    if (initPos)
    {
        fetchResults(gm, initPos);
    }

    GridPosition* result;

    int minHops = -1;

    for (int i = 0; i < gm->idxResults; i++)
    {
        Coordinate coor   = gm->results[i];
        GridPosition* pos = getGridPosition(gm, coor.x, coor.y);

        if (pos != NULL && pos->numHops < minHops || minHops == -1)
        {
            result  = pos;
            minHops = pos->numHops;
        }
    }

    return result;
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


int addResults(GameMap* gm, GridPosition* pos)
{
    if (gm->idxResults + 1 < MAX_RESULTS)
    {
        gm->results[gm->idxResults].x = pos->x;
        gm->results[gm->idxResults].y = pos->y;

        gm->idxResults += 1;

        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

int addToVisit(GameMap* gm, GridPosition* pos)
{
    if (gm->idxToVisit + 1 < MAX_TO_VISIT)
    {
        gm->toVisit[gm->idxToVisit].x = pos->x;
        gm->toVisit[gm->idxToVisit].y = pos->y;

        gm->idxToVisit += 1;

        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}