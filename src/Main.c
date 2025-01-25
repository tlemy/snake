#include <ncurses.h>
#include <stdio.h>

#include "../include/Init.h"
#include "../include/Draw.h"
#include "../include/Control.h"

#define CONTROL_C_KEY 3
#define LINE_FEED_KEY 10
#define CARRIAGE_RETURN_KEY 13
#define SLEEP_TIME 50 // millisecs
#define N_APPLES 24
#define N_BOTS 1
#define N_PLAYERS 1 + N_BOTS

void updateApples(Apple* apls);

void updatePlayers(Player* pls, Limit* lim, Apple* apls, int c);

void updateBorders(Player* human, Limit* lim);

void initPlayers(Player* pls, Limit* lim);

void initApples(Apple* apls, Limit* lim);

void freeSnakes(Player* pls);

void freeApples(Apple* apls);

// make snakes spawn more evenly spread out
// fix resetGrid

int main (void)
{
    setup();

    Limit lim;
    initLimits(&lim);

    Player pls[N_PLAYERS] = {};
    initPlayers(pls, &lim);

    Apple apls[N_APPLES] = {};
    initApples(apls, &lim);

    Player* human = &(pls[0]);
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
            initPlayer(human, 0, &lim);
            human->isHuman = 1;
        }

        erase();
        updateBorders(human, &lim);
        updateApples(apls);
        updatePlayers(pls, &lim, apls, c);
        refresh();

        napms(1000 / 20);
    }
}

void updatePlayers(Player* pls, Limit* lim, Apple* apls, int c)
{
    for (int i = 0; i < N_PLAYERS; i++)
    {
        Player* ply = &(pls[i]);

        // self collision, border collision
        if (!ply->isDead && (isBorderCollision(ply->snk, lim)
                || isCollidingWithSelf(ply->snk, lim)))
        {
            ply->isDead = 1;
        }

        // other players collision
        for (int j = 0; j < N_PLAYERS; j++)
        {
            if (i == j)
            {
                continue;
            }

            Player* other = &(pls[j]);

            if (!ply->isDead && isCollidingWithOther(ply->snk, other->snk, lim))
            {
                ply->isDead = 1;
            }
        }

        // control
        if (!ply->isHuman)
        {
            controlAutomatically(ply, lim);
            resetGrid(lim);
        }
        else
        {
            controlManually(c, ply);
        }

        // apples collision
        for (int i = 0; i < N_APPLES; i++)
        {
            Apple* apl = &(apls[i]);

            if (!ply->isDead && isAppleCollision(ply->snk, apl))
            {
                growSnake(ply->snk);
                apl->isEaten = 1;
                ++ply->score;
            }
        }

        // mouvement
        if (!ply->isDead)
        {
            moveSnake(ply->snk, getXInc(ply), getYInc(ply));
        }

        // draw snake
        if (!ply->isDead)
        {
            drawShape(ply->snk->head, ply->snk->len, getPlayerSnakeColorPair());
        }
    }
}

void updateApples(Apple* apls)
{
    for (int i = 0; i < N_APPLES; i++)
    {
        Apple* apl = &(apls[i]);

        if (apl->isEaten)
        {
            while(spawnApple(apl));
            apl->isEaten = 0;
        }
        drawShape(apl->shp, 1, getAppleColorPair());
    }
}

void updateBorders(Player* human, Limit* lim)
{
    if (!human->isDead)
    {
        drawBorders(lim->maxX, lim->maxY, getBorderColorPair(), human->score);
    }
    else
    {
        drawBorders(lim->maxX, lim->maxY, getAppleColorPair(), human->score);
    }
}

void initPlayers(Player* pls, Limit* lim)
{
    for (int i = 0; i < N_PLAYERS; i++)
    {
        Player* ply = &(pls[i]);
        initPlayer(ply, i * 10, lim);
    }
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