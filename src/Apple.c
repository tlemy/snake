#include "../include/Apple.h"
#include <stdlib.h>
#include <stdio.h>

Apple* newApple(int minX, int minY, int maxX, int maxY)
{
    Apple* apl = (Apple*) malloc(sizeof(Apple));
    apl->shp = newShape(newUnit(0, 0, SQUARE));
    apl->minX = minX;
    apl->minY = minY;
    apl->maxX = maxX;
    apl->maxY = maxY;

    return apl;
}

/*
when the limits are too small, the apple can spaw at the same place consecutively
this gives the impression it wasn't collected
this function should return 1 if this is the case
and 0 if the new position is different
*/
int spawnApple(Apple *apl)
{
    int randX = rand() % (apl->maxX - apl->minX + 1) + apl->minX;
    int randY = rand() % (apl->maxY - apl->minY + 1) + apl->minY;

    int minIsEven = apl->minX % 2 == 0;
    int randXIsEven = randX % 2 == 0;
    int diff = randX + 1 <= apl->maxX ? 1 : -1;

    /* To align the apple*/
    if ((minIsEven && !randXIsEven) || (!minIsEven && randXIsEven))
    {
        randX += diff;
    }

    int prevX = apl->shp->unt->x;
    int prevY =  apl->shp->unt->y;

    apl->shp->unt->x = randX;
    apl->shp->unt->y = randY;

    return prevX == randX && prevY == randY;
}

int freeApple(Apple *apl)
{
    if (freeShape(apl->shp))
    {
        free(apl);
        return 0;
    }
    return -1;
}