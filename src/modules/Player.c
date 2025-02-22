#include "Player.h"
#include "Direction.h"
#include "Incrementation.h"
#include "Key.h"
#include "Color.h"

#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

Player* newPlayer(int initLen, int x, int y, Direction dir)
{
    Player *pl   = calloc(1, sizeof(Player));

    if (!pl)
    {
        perror("newPlayer");
        exit(-1);
    }

    pl->snk      = newSnake(x, y, initLen, dir);
    pl->dir    = dir;
    pl->isDead   = 0;
    pl->score    = 0;
    pl->isHuman  = 0;
    pl->xTarget  = 0;
    pl->yTarget  = 0;
    pl->numMoves = 0;
    pl->moveIdx  = 0;

    return pl;
}

int getYIncPlayer(Player* pl)
{
    if (pl->dir == NORTH)
    {
        return -1 * Y_INC_SNAKE;
    }
    else if (pl->dir == SOUTH)
    {
        return Y_INC_SNAKE;
    }
    return 0;
}

int getXIncPlayer(Player* pl)
{
    if (pl->dir == WEST)
    {
        return -1 * X_INC_SNAKE;
    }
    else if (pl->dir == EAST)
    {
        return X_INC_SNAKE;
    }
    return 0;
}

void controlPlayer(Player* pl, Key k)
{
    if (k == NORTH_KEY && pl->dir != SOUTH)
    {
        pl->dir = NORTH;
    }
    else if (k == SOUTH_KEY && pl->dir != NORTH)
    {
        pl->dir = SOUTH;
    }
    else if (k == WEST_KEY && pl->dir != EAST)
    {
        pl->dir = WEST;
    }
    else if (k == EAST_KEY && pl->dir != WEST)
    {
        pl->dir = EAST;
    }
}

int freePlayer(Player* ply)
{
    int len = ply->snk->len;

    if (0 != freeSnake(ply->snk))
    {
        return -1;
    }
    free(ply);

    return 0;
}

void drawPlayer(Player* ply)
{
    if (!ply->isDead)
    {
        if (!ply->isHuman)
        {
            drawShape(ply->snk->head, ply->snk->len, BLUE_BLUE);
        }
        else
        {
            drawShape(ply->snk->head, ply->snk->len, GREEN_GREEN);
        }
    }
}