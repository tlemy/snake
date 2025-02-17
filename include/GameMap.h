#pragma once

#include "Direction.h"

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
    Direction dir;
    int numHops;
    char path;
    int x;
    int y;
} GridPosition;

typedef struct GameMap
{
    int minX;
    int minY;
    int maxX;
    int maxY;
    struct GridPosition** grid;
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

int freeGameMap(GameMap* gm);

int isBorderCollision(GameMap* gm, int x, int y);

void drawBorders(int maxX, int maxY, int pair, int score);

GridPosition* getGridPosition(GameMap* gm, int x, int y);

GridPosition* setGridPosition(GameMap* gm, int x, int y, int type);

GridPosition* scan(GameMap* gm, int x, int y, int debugX, int debugY);

GridPositionList* newList();

GridPositionList* addElementToList(GridPositionList* list, GridPosition* pos);

void freeList(GridPositionList* list);

GridPositionList* removeFirstElementFromList(GridPositionList* list);
