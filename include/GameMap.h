#pragma once

#include "Direction.h"

#define MAX_HOPS 100
#define MAX_LEN ((MAX_HOPS + 1) * 2) * ((MAX_HOPS + 1) * 2) * 2

typedef enum PostionType
{
    IS_FREE = 0,
    IS_APPLE  = 1,
    IS_SNAKE  = 2,
    IS_VISITED = 3,
} PostionType;

typedef struct GridPosition
{
    PostionType type;
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
    GridPosition** grid;
} GameMap;

typedef struct Coordinate
{
    int x;
    int y;
} Coordinate;

typedef struct CoordinateList
{
    int idxAdd;
    Coordinate arr[MAX_LEN];
} CoordinateList;

GameMap* newGameMap(int minX, int minY, int maxX, int maxY);

void resetGridGameMap(GameMap* gm);

void freeGameMap(GameMap* gm);

int isBorderCollision(GameMap* gm, int x, int y);

void drawBorders(int maxX, int maxY, int pair, int score);

GridPosition* getGridPosition(GameMap* gm, int x, int y);

void setGridPosition(GameMap* gm, int x, int y, int type);

GridPosition* scan(GameMap* gm, int x, int y);

GridPosition* target(GameMap* gm, int x, int y, int xTarget, int yTarget);

CoordinateList* addElementToList(CoordinateList* list, GridPosition* pos);
