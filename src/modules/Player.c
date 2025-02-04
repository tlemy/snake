#include "Player.h"
#include "Direction.h"
#include "Incrementation.h"

#include <string.h>
#include <stdlib.h>

Player* newPlayer(int initLen, int x, int y, int dirct)
{
    Player *pl   = (Player*) malloc(sizeof(Player));
    pl->snk      = newSnake(x, y, initLen, dirct);
    pl->dirct    = dirct;
    pl->isDead   = 0;
    pl->score    = 0;
    pl->isHuman  = 0;
    pl->numMoves = 0;
    pl->moveIdx  = 0;

    return pl;
}

int getYIncPlayer(Player* pl)
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

int getXIncPlayer(Player* pl)
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

void controlPlayer(Player* pl, int c)
{
    if (c == NORTH_KEY && pl->dirct != SOUTH)
    {
        pl->dirct = NORTH;
    }
    else if (c == SOUTH_KEY && pl->dirct != NORTH)
    {
        pl->dirct = SOUTH;
    }
    else if (c == WEST_KEY && pl->dirct != EAST)
    {
        pl->dirct = WEST;
    }
    else if (c == EAST_KEY && pl->dirct != WEST)
    {
        pl->dirct = EAST;
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