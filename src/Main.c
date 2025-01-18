#include <ncurses.h>

#include "../include/Init.h"
#include "../include/Collision.h"
#include "../include/Draw.h"
#include "../include/Control.h"

#define CONTROL_C_KEY 3
#define LINE_FEED_KEY 10
#define CARRIAGE_RETURN_KEY 13
#define SLEEP_TIME 50 // millisecs

int main (void)
{
    setup();

    int gameOver = false;
    Player p1;
    Limit lim;

    initLimits(&lim);
    initPlayer(&p1);

    p1.snk = newSnake(lim.minX, lim.minY, lim.maxX / 10);
    Apple* apl = newApple(lim.minX, lim.minY, lim.maxX - 2, lim.maxY -1);

    spawnApple(apl);

    while(1)
    {
        // controls
        int c = getch();

        if (CONTROL_C_KEY == c)
        {
            freeSnake(p1.snk);
            freeApple(apl);
            endwin(); // free resources and disable curses mode
            break;
        }

        if (!gameOver)
        {
            controlManually(c, &p1);
        }
        else
        {
            if (CARRIAGE_RETURN_KEY == c || LINE_FEED_KEY == c)
            {
                gameOver = false;
                initPlayer(&p1);
                initLimits(&lim);
                p1.snk = newSnake(lim.minX, lim.minY, lim.maxX / 10);
                p1.score = 0;
            }
        }

        // movement
        if (!gameOver)
        {
            moveSnake(p1.snk, getXInc(&p1), getYInc(&p1));
        }

        // collision
        if (!gameOver && (isBorderCollision(p1.snk, lim.minX, lim.minY, lim.maxX, lim.maxY)
                || isCollidingWithSelf(p1.snk)))
        {
            gameOver = true;
        }

        if (isAppleCollision(p1.snk, apl))
        {
            growSnake(p1.snk);

            while(spawnApple(apl));

            ++p1.score;
        }

        // display
        if (!gameOver)
        {
            erase();
            drawBorders(lim.maxX, lim.maxY, getBorderColorPair(), p1.score);
            drawShape(p1.snk->head, p1.snk->len, getPlayerSnakeColorPair());
            drawShape(apl->shp, 1, getAppleColorPair());
            refresh();
        }
        else
        {
            drawBorders(lim.maxX, lim.maxY, getAppleColorPair(), p1.score);
        }
        napms(1000 / 20);
    }
}