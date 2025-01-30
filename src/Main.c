#include <ncurses.h>
#include <stdio.h>

#include "../include/Init.h"
#include "../include/Apple.h"
#include "../include/Player.h"
#include "../include/Limit.h"


void addPlayersToGrid(Player* pls, Limit* lim);

void updatePlayers(Player* pls, Limit* lim, Apple* apls, int c);

void checkSnakesForCollision(Player* pls, Limit* lim, int i);

void checkApplesForCollision(Player* ply, Apple* apls);

void drawSnake(Player* ply);

void updateApples(Apple* apls, Limit* lim);

void updateBorders(Player* human, Limit* lim);

void initPlayers(Player* pls, Limit* lim);

void initHuman(Player* pls, Limit* lim);

void initApples(Apple* apls, Limit* lim);

void freeSnakes(Player* pls);

void freeApples(Apple* apls);

int isCollidingWithOther(Snake* snk1, Snake* snk2, Limit* lim);


int main (void)
{
    setup();

    Limit lim;
    initLimits(&lim);

    Player pls[N_PLAYERS] = {};
    initPlayers(pls, &lim);

    Apple apls[N_APPLES] = {};
    initApples(apls, &lim);

    Player* human  = &(pls[0]);
    human->isHuman = 1;

    while(1)
    {
        int c = getch();

        // monitor for exit
        if (CONTROL_C_KEY == c)
        {
            freeSnakes(pls);
            freeApples(apls);
            freeGrid(&lim);
            endwin(); // free resources and disable curses mode
            return 0;
        }

        // monitor for respawn
        if (human->isDead && (CARRIAGE_RETURN_KEY == c || LINE_FEED_KEY == c))
        {
            initHuman(pls, &lim);
            human->isHuman = 1;
        }

        erase();
        updateBorders(human, &lim);
        updateApples(apls, &lim);
        addPlayersToGrid(pls, &lim);
        updatePlayers(pls, &lim, apls, c);
        resetGrid(&lim);
        refresh();

        napms(1000 / 20);
    }
}

void addPlayersToGrid(Player* pls, Limit* lim)
{
    for (int i = 0; i < N_PLAYERS; i++)
    {
        Player* ply = &(pls[i]);
        Shape* sh   = ply->snk->head;

        for (int j = 0; j < pls->snk->len; j++)
        {
            lim->grid[sh->x * sh->y] = IS_SNAKE;

            if (sh->nxt != NULL)
            {
                sh = sh->nxt;
            }
        }
    }
}

void updatePlayers(Player* pls, Limit* lim, Apple* apls, int c)
{
    for (int i = 0; i < N_PLAYERS; i++)
    {
        Player* ply = &(pls[i]);
        Shape head  = *(ply->snk->head);
        int x       = head.x;
        int y       = head.y;
        int len     = ply->snk->len;

        if (ply->isDead)
        {
            continue;
        }

        checkSnakesForCollision(pls, lim, i);
        controlPlayer(ply, c);
        checkApplesForCollision(ply, apls);
        moveSnake(ply->snk, getXIncPlayer(ply), getYIncPlayer(ply));
        drawSnake(ply);
    }
}

void checkSnakesForCollision(Player* pls, Limit* lim, int i)
{
    Player* ply = &(pls[i]);
    Shape head  = *(ply->snk->head);
    int x       = head.x;
    int y       = head.y;
    int len     = ply->snk->len;

    if (isBorderCollision(lim, x, y)
                || isCollidingWithPoint(*(ply->snk->head->nxt), x, y, len - 1))
    {
        ply->isDead = 1;
    }

    for (int j = 0; j < N_PLAYERS; j++)
    {
        if (i == j)
        {
            continue;
        }

        Player* other = &(pls[j]);

        if (other->isDead)
        {
            continue;
        }

        if (!ply->isDead && isCollidingWithOther(ply->snk, other->snk, lim))
        {
            ply->isDead = 1;
        }
    }
}

void checkApplesForCollision(Player* ply, Apple* apls)
{
    int playerX = ply->snk->head->x;
    int playerY = ply->snk->head->y;

    for (int i = 0; i < N_APPLES; i++)
    {
        Apple* apl = &(apls[i]);
        int appleX = apl->shp->x;
        int appleY = apl->shp->y;

        if (!ply->isDead && appleX == playerX && appleY == playerY)
        {
            growSnake(ply->snk);
            apl->isEaten = 1;
            ++ply->score;
        }
    }
}

void drawSnake(Player* ply)
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

void updateApples(Apple* apls, Limit* lim)
{
    for (int i = 0; i < N_APPLES; i++)
    {
        Apple* apl = &(apls[i]);

        if (apl->isEaten)
        {
            while(spawnApple(apl));
            apl->isEaten = 0;
        }
        drawShape(apl->shp, 1, RED_RED);
        lim->grid[apl->shp->x * apl->shp->y] = IS_APPLE;
    }
}

void updateBorders(Player* human, Limit* lim)
{
    if (!human->isDead)
    {
        drawBorders(lim->maxX, lim->maxY, BLACK_WHITE, human->score);
    }
    else
    {
        drawBorders(lim->maxX, lim->maxY, RED_RED, human->score);
    }
}

void initPlayers(Player* pls, Limit* lim)
{
    initHuman(pls, lim);

    Player* bot1 = &(pls[1]);
    initPlayer(bot1, lim->maxX / 30, lim->maxX - (X_INC_SNAKE * 2), lim->maxY / 2, WEST);

    Player* bot2 = &(pls[2]);
    initPlayer(bot2, lim->maxX / 30, lim->maxX / 2, lim->minY, SOUTH);

    Player* bot3 = &(pls[3]);
    initPlayer(bot3, lim->maxX / 30, lim->maxX / 2, lim->maxY - (Y_INC_SNAKE * 2), NORTH);
}

void initHuman(Player* pls, Limit* lim)
{
    Player* human = &(pls[0]);
    initPlayer(human, lim->maxX / 30, lim->minX, lim->maxY / 2, EAST);
}

void initApples(Apple* apls, Limit* lim)
{
    for (int i = 0; i < N_APPLES; i++)
    {
        Apple* apl = &(apls[i]);
        initApple(apl, lim->minX, lim->minY - 1, lim->maxX - 1, lim->maxY - 1);
        spawnApple(apl);
    }
}

void freeSnakes(Player* pls)
{
    for (int i = 0; i < N_PLAYERS; i++)
    {
        Player* ply = &(pls[i]);
        freeSnake(ply->snk);
    }
}

void freeApples(Apple* apls)
{
    for (int i = 0; i < N_APPLES; i++)
    {
        Apple* apl = &(apls[i]);
        freeApple(apl);
    }
}

int isCollidingWithOther(Snake* snk1, Snake* snk2, Limit* lim)
{
    Shape* head  = snk1->head;
    Shape* other = snk2->head;

    for (int i = 1; i < snk2->len; i++)
    {
        if (head->x == other->x && head->y == other->y)
        {
            return 1;
        }

        other = other->nxt;
    }
    return 0;
}