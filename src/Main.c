#include <ncurses.h>

#include "../include/Init.h"
#include "../include/Collision.h"
#include "../include/Draw.h"
#include "../include/Control.h"

#define CONTROL_C_KEY 3
#define LINE_FEED_KEY 10
#define CARRIAGE_RETURN_KEY 13
#define SLEEP_TIME 50 // millisecs
#define N_BOTS 3
#define N_PLAYERS 1 + N_BOTS

// spread out the bots at 4 corners
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

    Apple* apl = newApple(lim.minX, lim.minY - 1, lim.maxX - 1, lim.maxY - 1);
    spawnApple(apl);

    Player* human = &(pls[0]);

    while(1)
    {
        // controls
        int c = getch();

        if (CONTROL_C_KEY == c)
        {
            for (int i = 0; i < N_PLAYERS; i++)
            {
                Player* ply = &(pls[i]);
                freeSnake(ply->snk);
            }

            freeApple(apl);
            endwin(); // free resources and disable curses mode
            break;
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
                    initPlayer(ply, i * marginTop, &lim);
                }
            }
        }

        erase();

        for (int i = 0; i < N_PLAYERS; i++)
        {
            Player* ply = &(pls[i]);

            // movement
            if (!ply->isDead)
            {
                moveSnake(ply->snk, getXInc(ply), getYInc(ply));
            }

            // collision
            if (!ply->isDead && (isBorderCollision(ply->snk, lim.minX, lim.minY, lim.maxX, lim.maxY)
                    || isCollidingWithSelf(ply->snk)))
            {
                ply->isDead = 1;
            }

            if (isAppleCollision(ply->snk, apl))
            {
                growSnake(ply->snk);

                while(spawnApple(apl));

                ++ply->score;
            }

            // draw snake
            drawShape(ply->snk->head, ply->snk->len, getPlayerSnakeColorPair());
        }

        // draw apple
        drawShape(apl->shp, 1, getAppleColorPair());

        // draw borders
        if (!human->isDead)
        {
            drawBorders(lim.maxX, lim.maxY, getBorderColorPair(), human->score);
        }
        else
        {
            drawBorders(lim.maxX, lim.maxY, getAppleColorPair(), human->score);
        }

        refresh();
        napms(1000 / 20);
    }
}