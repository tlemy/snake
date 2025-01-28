#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../include/Control.h"

int getYInc(Player* pl)
{
    if (pl->dirct == NORTH)
    {
        return -1 * Y_INC_SNAKE;
    }
    else if (pl->dirct == SOUTH)
    {
        return Y_INC_SNAKE;
    }
    return 0;
}

int getXInc(Player* pl)
{
    if (pl->dirct == WEST)
    {
        return -1 * X_INC_SNAKE;
    }
    else if (pl->dirct == EAST)
    {
        return X_INC_SNAKE;
    }
    return 0;
}

void  initPlayer(Player* pl, Limit* lim, int x, int y, int dirct)
{
    pl->dirct = dirct;
    pl->snk = newSnake(x, y, lim->maxX / 30, dirct);
    pl->isDead = 0;
    pl->score = 0;
    pl->isHuman = 0;
}

void initLimits(Limit* lim)
{
    lim->minX = 1;
    lim->minY = 2;

    getmaxyx(stdscr, lim->maxY, lim->maxX);
    lim->maxY -= 1;
    lim->maxX -= 1;

    /*
    * The squares being displayed take 1 row and 2 columns. ("  ")
    * As a consequence, the number of colums has to be pair.
    */
    if ((lim->maxX - 1) % 2 != 0)
    {
        lim->maxX -= 1;
    }

    lim->grid = (int*) malloc(lim->maxX * lim->maxY * sizeof(int));
}

void resetGrid(Limit* lim)
{
    memset(lim->grid, 0, lim->maxX * lim->maxY * (sizeof(int)));
}

void freeGrid(Limit* lim)
{
    free(lim->grid);
}

void controlManually(int c, Player* pl)
{
    if (c == W_KEY && pl->dirct != SOUTH)
    {
        pl->dirct = NORTH;
    }
    else if (c == S_KEY && pl->dirct != NORTH)
    {
        pl->dirct = SOUTH;
    }
    else if (c == A_KEY && pl->dirct != EAST)
    {
        pl->dirct = WEST;
    }
    else if (c == D_KEY && pl->dirct != WEST)
    {
        pl->dirct = EAST;
    }
}

void controlAutomatically(Player* pl, Limit* lim)
{
    // Shape* head = pl->snk->head;
    // implement algo to avoid borders
    // implement algo to avoid player in grid
    // implement algo to seek apples
}

int isBorderCollision(Snake *snk, Limit* lim)
{
    int leftBorderCollision = snk->head->unt->x < lim->minX;
    int topBorderCollision = snk->head->unt->y < lim->minY;
    int rightBorderCollision = snk->head->unt->x >= lim->maxX - 2; // each unit is 2 characters long
    int bottomBorderCollision = snk->head->unt->y >= lim->maxY - 1; // each unit is one character high

    return leftBorderCollision || rightBorderCollision || topBorderCollision || bottomBorderCollision;
}

int isAppleCollision(Snake* snk, Apple* apl)
{
    int isXEqual = snk->head->unt->x == apl->shp->unt->x;
    int isYEqual = snk->head->unt->y == apl->shp->unt->y;

    return isXEqual && isYEqual;
}

int isCollidingWithSelf(Snake* snk, Limit* lim)
{
    Shape *head = snk->head;
    Shape *body = head->nxt;

    for (int i = 1; i < snk->len; i++)
    {
        if (head->unt->x == body->unt->x && head->unt->y == body->unt->y)
        {
            return 1;
        }

        body = body->nxt;
    }
    return 0;
}

int isCollidingWithOther(Snake* snk1, Snake* snk2, Limit* lim)
{
    Shape* head = snk1->head;
    Shape* other = snk2->head;

    for (int i = 1; i < snk2->len; i++)
    {
        if (head->unt->x == other->unt->x && head->unt->y == other->unt->y)
        {
            return 1;
        }

        other = other->nxt;
    }
    return 0;
}