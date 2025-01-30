#pragma once

typedef struct Limit
{
    int minX;
    int minY;
    int maxX;
    int maxY;
    int* grid;
} Limit;

void initLimits(Limit* lim);

void resetGrid(Limit* lim);

void freeGrid(Limit* lim);

int isBorderCollision(Limit* lim, int x, int y);