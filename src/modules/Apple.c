#include "Apple.h"
#include "Incrementation.h"

#include <stdlib.h>
#include <stdio.h>

#define SQUARE "  "

Apple* newApple(int minX, int minY, int maxX, int maxY)
{
    Apple* apl = (Apple*) malloc(sizeof(Apple));
    apl->shp     = newShape(0, 0, SQUARE);
    apl->minX    = minX + X_INC_SNAKE;
    apl->minY    = minY + Y_INC_SNAKE;
    apl->maxX    = maxX - X_INC_SNAKE;
    apl->maxY    = maxY - Y_INC_SNAKE;
    apl->isEaten = 1;

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

    int minIsEven   = apl->minX % 2 == 0;
    int randXIsEven = randX % 2 == 0;
    int diff        = randX + 1 <= apl->maxX ? 1 : -1;

    /* To align the apple*/
    if ((minIsEven && !randXIsEven) || (!minIsEven && randXIsEven))
    {
        randX += diff;
    }

    int prevX = apl->shp->x;
    int prevY =  apl->shp->y;

    apl->shp->x = randX;
    apl->shp->y = randY;

    return prevX == randX && prevY == randY;
}

int freeApple(Apple *apl)
{
    if (freeShape(apl->shp))
    {
        return 0;
    }
    return -1;
}

void freeApples(Apple** apls, int n)
{
    for (int i = 0; i < n; i++)
    {
        freeApple(apls[i]);
    }
    free(apls);
}