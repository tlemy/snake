#include <ncurses.h>
#include "../include/Position.h"

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

void initPositions(Player* pl)
{
    pl->left = 0;
    pl->right = 1;
    pl->up = 0;
    pl->down = 0;
}

void initLimits(Limit* lim)
{
    lim->minX = 1;
    lim->minY = 1;

    getmaxyx(stdscr, lim->maxY, lim->maxX);
    lim->maxY -= 1; // rows start at idx 0
    lim->maxX -= 1; // columns start at idx 0

    /*
    * The squares being displayed take 1 row and 2 columns. ("  ")
    * As a consequence, the number of colums has to be pair.
    */
    if ((lim->maxX - 1) % 2 != 0)
    {
        lim->maxX -= 1;
    }
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