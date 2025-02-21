#pragma once

#include "Direction.h"

#define MAX_HOPS     25
#define MAX_RESULTS  (MAX_HOPS * 2) * (MAX_HOPS * 2)
#define MAX_TO_VISIT (MAX_HOPS * 2) * (MAX_HOPS * 2)

typedef enum PostionType
{
    IS_FREE = 0,
    IS_APPLE  = 1,
    IS_SNAKE  = 2,
    IS_VISITED = 3,
    IS_ORIGINAL = 4
} PostionType;

typedef struct GridPosition
{
    PostionType type;
    int numHops;
    char path;
    int x;
    int y;
} GridPosition;

typedef struct Coordinate
{
    int x;
    int y;
} Coordinate;

typedef struct GameMap
{
    int minX;
    int minY;
    int maxX;
    int maxY;
    int idxResults;
    int idxToVisit;
    GridPosition** grid;
    Coordinate results[MAX_RESULTS];
    Coordinate toVisit[MAX_RESULTS];
} GameMap;

typedef struct GridPositionElement
{
    GridPosition* pos;
    struct GridPositionElement* next;
} GridPositionElement;

typedef struct GridPositionList
{
    GridPositionElement* head;
    GridPositionElement* tail;
} GridPositionList;

GameMap* newGameMap(int minX, int minY, int maxX, int maxY);

void resetGridGameMap(GameMap* gm);

void freeGameMap(GameMap* gm);

int isBorderCollision(GameMap* gm, int x, int y);

void drawBorders(int maxX, int maxY, int pair, int score);

GridPosition* getGridPosition(GameMap* gm, int x, int y);

void setGridPosition(GameMap* gm, int x, int y, int type);

GridPosition* scan(GameMap* gm, int x, int y);

GridPositionList* newList();

GridPositionList* addElementToList(GridPositionList* list, GridPosition* pos);

void freeList(GridPositionList* list);

GridPositionList* removeFirstElementFromList(GridPositionList* list);

int addResults(GameMap* gm, GridPosition* pos);

int addToVisit(GameMap* gm, GridPosition* pos);
