#include "Apple.h"
#include "Player.h"
#include "Limit.h"
#include "Direction.h"
#include "Incrementation.h"
#include "Colors.h"
#include "Params.h"

#include <ncurses.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void addPlayersToGrid(Player** pls, Limit* lim);

void updatePlayers(Player** pls, Limit* lim, Apple** apls, int c);

void checkSnakesForCollision(Player** pls, Limit* lim, int i);

void checkApplesForCollision(Player* ply, Apple** apls);

void drawSnake(Player* ply);

void updateApples(Apple** apls, Limit* lim);

void updateBorders(Player* human, Limit* lim);

void initPlayers(Player** pls, Limit* lim);

void initApples(Apple** apls, Limit* lim);

void freeSnakes(Player** pls);

void freeApples(Apple** apls);

int isCollidingWithOther(Snake* snk1, Snake* snk2, Limit* lim);

void drawBorders(int maxX, int maxY, int pair, int scr);

void drawShape(Shape *shp, int len, int pair);

void setup(void)
{
    initscr(); // start curses mode
    raw(); // disable buffering for input, keyboard interrupt passed as characters without generating a signal
    noecho(); // turn off printing the characters selected by the user
    curs_set(0); // hide cursor
    start_color(); // enable color capabilitie
    nodelay(stdscr, TRUE); // avoid getch blocking
    init_pair(WHITE_WHITE, COLOR_WHITE, COLOR_WHITE); // border color
    init_pair(RED_RED, COLOR_RED, COLOR_RED); // apple color
    init_pair(GREEN_GREEN, COLOR_GREEN, COLOR_GREEN); // snake color
    init_pair(BLACK_WHITE, COLOR_BLACK, COLOR_WHITE); // score color
    init_pair(BLUE_BLUE, COLOR_CYAN, COLOR_CYAN); // enemies
}

int main (void)
{
    setup();

    Limit lim;
    initLimits(&lim);

    Player** pls = (Player**) malloc(N_PLAYERS * sizeof(Player*));
    initPlayers(pls, &lim);

    Apple** apls = (Apple**) malloc(N_APPLES * sizeof(Apple*));
    initApples(apls, &lim);

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
        if (pls[0]->isDead && (CARRIAGE_RETURN_KEY == c || LINE_FEED_KEY == c))
        {
            free(pls[0]);
            pls[0] = newPlayer(lim.maxX / 30, lim.minX, lim.maxY / 2, EAST);
            pls[0]->isHuman = 1;
        }

        erase();
        updateBorders(pls[0], &lim);
        updateApples(apls, &lim);
        addPlayersToGrid(pls, &lim);
        updatePlayers(pls, &lim, apls, c);
        resetGrid(&lim);
        refresh();

        napms(1000 / 20);
    }
}

void addPlayersToGrid(Player** pls, Limit* lim)
{
    for (int i = 0; i < N_PLAYERS; i++)
    {
        Player* ply = pls[i];
        Shape* sh   = ply->snk->head;

        for (int j = 0; j < ply->snk->len; j++)
        {
            lim->grid[sh->x * sh->y] = IS_SNAKE;

            if (sh->nxt != NULL)
            {
                sh = sh->nxt;
            }
        }
    }
}

void updatePlayers(Player** pls, Limit* lim, Apple** apls, int c)
{
    for (int i = 0; i < N_PLAYERS; i++)
    {
        Player* ply = pls[i];
        Shape* head = ply->snk->head;
        int x       = head->x;
        int y       = head->y;
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

void checkSnakesForCollision(Player** pls, Limit* lim, int i)
{
    Player* ply = pls[i];
    Shape* head = ply->snk->head;
    int x       = head->x;
    int y       = head->y;
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

        Player* other = pls[j];

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

void checkApplesForCollision(Player* ply, Apple** apls)
{
    int playerX = ply->snk->head->x;
    int playerY = ply->snk->head->y;

    for (int i = 0; i < N_APPLES; i++)
    {
        Apple* apl = apls[i];
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

void updateApples(Apple** apls, Limit* lim)
{
    for (int i = 0; i < N_APPLES; i++)
    {
        Apple* apl = apls[i];

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

void initPlayers(Player** pls, Limit* lim)
{
    pls[0] = newPlayer(lim->maxX / 30, lim->minX, lim->maxY / 2, EAST);
    pls[1] = newPlayer(lim->maxX / 30, lim->maxX - (X_INC_SNAKE * 2), lim->maxY / 2, WEST);
    pls[2] = newPlayer(lim->maxX / 30, lim->maxX / 2, lim->minY, SOUTH);
    pls[3] = newPlayer(lim->maxX / 30, lim->maxX / 2, lim->maxY - (Y_INC_SNAKE * 2), NORTH);

    pls[0]->isHuman = 1;
}

void initApples(Apple** apls, Limit* lim)
{
    for (int i = 0; i < N_APPLES; i++)
    {
        apls[i] = newApple(lim->minX, lim->minY - 1, lim->maxX - 1, lim->maxY - 1);
        spawnApple(apls[i]);
    }
}

void freeSnakes(Player** pls)
{
    for (int i = 0; i < N_PLAYERS; i++)
    {
        Player* ply = pls[i];
        freeSnake(ply->snk);
    }
}

void freeApples(Apple** apls)
{
    for (int i = 0; i < N_APPLES; i++)
    {
        freeApple(apls[i]);
    }
    free(apls);
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

void drawShape(Shape *shp, int len, int pair)
{
    attron(COLOR_PAIR(pair));

    Shape* ptr = shp;

    for (int i = 0; i < len; i++)
    {
        mvaddstr(ptr->y, ptr->x, ptr->str);
        ptr = ptr->nxt;
    }

    attroff(COLOR_PAIR(pair));
}

void drawBorders(int maxX, int maxY, int pair, int scr)
{
    char str[32];
    sprintf(str, " [ SCORE: %d ] ", scr);
    attron(COLOR_PAIR(BLACK_WHITE));
    mvaddstr(0, 1, str);
    attroff(COLOR_PAIR(BLACK_WHITE));

    attron(COLOR_PAIR(pair));

    // top
    for (int x = strlen(str) + 1; x <= maxX ; x++)
    {
        mvaddch(0, x, SPACE);
    }

    // bottom
    for (int x = 0; x <= maxX; x++)
    {
        mvaddch(maxY, x, SPACE);
    }

    // left
    for (int y = 0; y < maxY; y++)
    {
        mvaddch(y, 0, SPACE);
    }

    // right
    for (int y = 0; y < maxY; y++)
    {
        mvaddch(y, maxX, SPACE);
    }

    attroff(COLOR_PAIR(pair));
}