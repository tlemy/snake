#include "../include/Snake.h"
#include "../include/Apple.h"

int isBorderCollision(Snake *snk, int minX, int minY, int maxX, int maxY)
{
    int leftBorderCollision = snk->head->unt->x < minX;
    int topBorderCollision = snk->head->unt->y < minY;
    int rightBorderCollision = snk->head->unt->x > maxX - 2; // each unit is 2 characters long
    int bottomBorderCollision = snk->head->unt->y > maxY - 1; // each unit is one character high

    return leftBorderCollision || rightBorderCollision || topBorderCollision || bottomBorderCollision;
}

int isAppleCollision(Snake* snk, Apple* apl)
{
    int isXEqual = snk->head->unt->x == apl->shp->unt->x;
    int isYEqual = snk->head->unt->y == apl->shp->unt->y;

    return isXEqual && isYEqual;
}