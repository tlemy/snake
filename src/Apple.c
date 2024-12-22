#include "../include/Apple.h"
#include <stdlib.h>

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

void spawnApple(Apple *apl) 
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

    apl->shp->unt->x = randX;
    apl->shp->unt->y = randY;
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