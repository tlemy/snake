#include <ncurses.h>

#include "../include/Init.h"
#include "../include/Collision.h"
#include "../include/Draw.h"
#include "../include/Control.h"

#define CONTROL_C_KEY 3
#define LINE_FEED_KEY 10
#define CARRIAGE_RETURN_KEY 13
#define SLEEP_TIME 50 // millisecs
#define N_APPLES 6
#define N_BOTS 3
#define N_PLAYERS 1 + N_BOTS

void updateApples(Apple* apls);

int controlMouvement(Player* pls, Limit* lim, Apple* apls);

void updatePlayers(Player* pls, Limit* lim, Apple* apls);

void updateBorders(Player* human, Limit* lim);

int main (void)
{
    setup();

    Limit lim;
    initLimits(&lim);

    int marginTop = (int) ((lim.maxY - lim.minY) / N_BOTS);

    Player pls[N_PLAYERS] = {};

    for (int i = 0; i < N_PLAYERS; i++)
    {
        Player* ply = &(pls[i]);
        initPlayer(ply, i * marginTop, &lim);
    }

    Apple apls[N_APPLES] = {};

    for (int i = 0; i < N_APPLES; i++)
    {
        Apple* apl = &(apls[i]);
        initApple(apl, lim.minX, lim.minY - 1, lim.maxX - 1, lim.maxY - 1);
        spawnApple(apl);
    }

    Player* human = &(pls[0]);

    while(controlMouvement(pls, &lim, apls))
    {
        erase();

        updateApples(apls);
        updateBorders(human, &lim);
        updatePlayers(pls, &lim, apls);

        refresh();
        napms(1000 / 20);
    }
}

void updateApples(Apple* apls)
{
    for (int i = 0; i < N_APPLES; i++)
    {
        Apple* apl = &(apls[i]);
        drawShape(apl->shp, 1, getAppleColorPair());
    }
}

int controlMouvement(Player* pls, Limit* lim, Apple* apls)
{
    int c = getch();
    int marginTop = (int) ((lim->maxY - lim->minY) / N_BOTS);
    Player* human = &(pls[0]);

    if (CONTROL_C_KEY == c)
    {
        for (int i = 0; i < N_PLAYERS; i++)
        {
            Player* ply = &(pls[i]);
            freeSnake(ply->snk);
        }

        for (int i = 0; i < N_APPLES; i++)
        {
            Apple* apl = &(apls[i]);
            freeApple(apl);
        }

        endwin(); // free resources and disable curses mode
        return 0;
    }

    if (!human->isDead)
    {
        controlManually(c, human);
    }
    else
    {
        if (CARRIAGE_RETURN_KEY == c || LINE_FEED_KEY == c)
        {
            for (int i = 0; i < N_PLAYERS; i++)
            {
                Player* ply = &(pls[i]);
                initPlayer(ply, i * marginTop, lim);
            }
        }
    }
    return 1;
}

void updatePlayers(Player* pls, Limit* lim, Apple* apls)
{
    for (int i = 0; i < N_PLAYERS; i++)
    {
        Player* ply = &(pls[i]);

        // movement
        if (!ply->isDead)
        {
            moveSnake(ply->snk, getXInc(ply), getYInc(ply));
        }

        // self collision, border collision
        if (!ply->isDead && (isBorderCollision(ply->snk, lim->minX, lim->minY, lim->maxX, lim->maxY)
                || isCollidingWithSelf(ply->snk)))
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
        }

        // apples collision
        for (int i = 0; i < N_APPLES; i++)
        {
            Apple* apl = &(apls[i]);

            if (isAppleCollision(ply->snk, apl))
            {
                growSnake(ply->snk);
                while(spawnApple(apl));
                ++ply->score;
            }
        }

        // draw snake
        drawShape(ply->snk->head, ply->snk->len, getPlayerSnakeColorPair());
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