#include "string.h"

#include "../include/Player.h"
#include "../include/Params.h"

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

void initPlayer(Player* pl, int initLen, int x, int y, int dirct)
{
    pl->dirct   = dirct;
    pl->snk     = newSnake(x, y, initLen, dirct);
    pl->isDead  = 0;
    pl->score   = 0;
    pl->isHuman = 0;
    pl->numMoves = 0;
    pl->moveIdx = 0;
}

void controlPlayer(Player* pl, int c)
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

