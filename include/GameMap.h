#pragma once

typedef enum PostionType
{
    IS_APPLE  = 1,
    IS_SNAKE  = 2
} PostionType;

typedef struct GameMap
{
    int minX;
    int minY;
    int maxX;
    int maxY;
    int** grid;
} GameMap;

GameMap* newGameMap(int minX, int minY, int maxX, int maxY);

void resetGridGameMap(GameMap* gm);

int freeGameMap(GameMap* gm);

int isBorderCollision(GameMap* gm, int x, int y);

void drawBorders(int maxX, int maxY, int pair, int score);

PostionType getAtGameMapPosition(GameMap* gm, int x, int y);

int setAtGameMapPosition(GameMap* gm, int x, int y, PostionType type);