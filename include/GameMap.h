#pragma once

#include "Direction.h"

typedef enum PostionType
{
    IS_FREE = 0,
    IS_APPLE  = 1,
    IS_SNAKE  = 2,
    IS_VISITED = 3
} PostionType;

typedef struct GridPosition
{
    int x;
    int y;
    int type;
    Direction dir;
    int parentX;
    int parentY;
} GridPosition;

typedef struct GameMap
{
    int minX;
    int minY;
    int maxX;
    int maxY;
    struct GridPosition** grid;
} GameMap;

GameMap* newGameMap(int minX, int minY, int maxX, int maxY);

void resetGridGameMap(GameMap* gm);

int freeGameMap(GameMap* gm);

int isBorderCollision(GameMap* gm, int x, int y);

void drawBorders(int maxX, int maxY, int pair, int score);

GridPosition* getGridPosition(GameMap* gm, int x, int y);

GridPosition* setGridPosition(GameMap* gm, int x, int y, int type);

GridPosition* scan(GameMap* gm, int x, int y);