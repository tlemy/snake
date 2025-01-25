#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../include/Control.h"

#define X_INC_SNAKE 2
#define Y_INC_SNAKE 1
#define W_KEY 'w'
#define A_KEY 'a'
#define S_KEY 's'
#define D_KEY 'd'

int getYInc(Player* pl)
{
    if (pl->up)
    {
        return -1 * Y_INC_SNAKE;
    }
    else if (pl->down)
    {
        return Y_INC_SNAKE;
    }
    return 0;
}

int getXInc(Player* pl)
{
    if (pl->left)
    {
        return -1 * X_INC_SNAKE;
    }
    else if (pl->right)
    {
        return X_INC_SNAKE;
    }
    return 0;
}

void  initPlayer(Player* pl, int marginTop, Limit* lim)
{
    int initX = lim->minX;
    int initY = lim->minY + marginTop;

    pl->left = 0;
    pl->right = 1;
    pl->up = 0;
    pl->down = 0;
    pl->snk = newSnake(initX, initY, lim->maxX / 20);
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
    if (c == W_KEY && !(pl->down))
    {
        pl->up = 1;
        pl->down = 0;
        pl->left = 0;
        pl->right = 0;
    }
    else if (c == S_KEY && !(pl->up))
    {
        pl->down = 1;
        pl->up = 0;
        pl->left = 0;
        pl->right = 0;
    }
    else if (c == A_KEY && !(pl->right))
    {
        pl->left = 1;
        pl->right = 0;
        pl->up = 0;
        pl->down = 0;
    }
    else if (c == D_KEY && !(pl->left))
    {
        pl->right = 1;
        pl->left = 0;
        pl->up = 0;
        pl->down = 0;
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