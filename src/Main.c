#include "Apple.h"
#include "Player.h"
#include "GameMap.h"
#include "Direction.h"
#include "Incrementation.h"
#include "Color.h"
#include "Key.h"

#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CONTROL_C_KEY 3
#define LINE_FEED_KEY 10
#define CARRIAGE_RETURN_KEY 13
#define SLEEP_TIME 50 // millisecs
#define N_APPLES 50
#define N_BOTS 3
#define N_PLAYERS 1 + N_BOTS

#define SPACE ' '

void addPlayersToGrid(Player* pls[N_PLAYERS], GameMap* gm);
void addApplesToGrid(Apple* apls[N_APPLES], GameMap* gm);

void updateApples(Apple* apls[N_APPLES], GameMap* gm);
void updateBorders(Player* pls[N_PLAYERS], GameMap* gm);
void updatePlayers(Player* pls[N_PLAYERS], GameMap* gm, Apple* apls[N_APPLES], int c);

void checkSnakesForCollision(Player* pls[N_PLAYERS], GameMap* gm, int i);
void checkApplesForCollision(Player* ply, Apple* apls[N_APPLES]);

void initPlayers(Player* pls[N_PLAYERS], GameMap* gm);
void initApples(Apple* apls[N_APPLES] , GameMap* gm);

int isCollidingWithOther(Snake* snk1, Snake* snk2);

void freeApples(Apple* apls[N_APPLES]);
void freePlayers(Player* pls[N_PLAYERS]);

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
    init_pair(MAGENTA_MAGENTA, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(YELLOW_YELLOW, COLOR_YELLOW, COLOR_YELLOW);
}

int main (void)
{
    setup();

    int maxX = getmaxx(stdscr);
    int maxY = getmaxy(stdscr);

    GameMap* gm = newGameMap(1, 1, maxX, maxY);

    Player* pls[N_PLAYERS];
    initPlayers(pls, gm);

    Apple* apls[N_APPLES];
    initApples(apls, gm);

    while(1)
    {
        int c = getch();

        // monitor for exit
        if (CONTROL_C_KEY == c)
        {
            freePlayers(pls);
            freeApples(apls);
            freeGameMap(gm);
            endwin(); // free resources and disable curses mode
            return 0;
        }

        // monitor for respawn
        if (pls[0]->isDead && (CARRIAGE_RETURN_KEY == c || LINE_FEED_KEY == c))
        {
            free(pls[0]);
            pls[0] = newPlayer(gm->maxX / 30, gm->minX, gm->maxY / 2, EAST);
            pls[0]->isHuman = 1;
        }

        erase();
        updateBorders(pls, gm);
        updateApples(apls, gm);
        updatePlayers(pls, gm, apls, c);
        refresh();

        napms(1000 / 25);
    }
}

void addPlayersToGrid(Player* pls[N_PLAYERS], GameMap* gm)
{
    for (int i = 0; i < N_PLAYERS; i++)
    {
        Player* ply = pls[i];

        if (ply->isDead)
        {
            continue;
        }

        Shape* sh   = ply->snk->head;

        for (int j = 0; j < ply->snk->len; j++)
        {
            setGridPosition(gm, sh->x, sh->y, IS_SNAKE);

            if (sh->nxt != NULL)
            {
                sh = sh->nxt;
            }
        }
    }
}

void addApplesToGrid(Apple* apls[N_APPLES], GameMap* gm)
{
    for (int i = 0; i < N_APPLES; i++)
    {
        Apple* apl = apls[i];
        Shape* sh   = apl->shp;

        setGridPosition(gm, sh->x, sh->y, IS_APPLE);
    }
}

void updatePlayers(Player* pls[N_PLAYERS], GameMap* gm, Apple* apls[N_APPLES], int c)
{
    for (int i = 0; i < N_PLAYERS; i++)
    {
        Player* ply = pls[i];

        if (ply->isDead)
        {
            continue;
        }

        Shape* head = ply->snk->head;
        int x       = head->x;
        int y       = head->y;

        if (!ply->isHuman)
        {
            GridPosition* pos = NULL;

            addPlayersToGrid(pls, gm);
            addApplesToGrid(apls, gm);

            pos = scan(gm, x, y, IS_APPLE, 0);

            if (pos != NULL)
            {
                attron(COLOR_PAIR(MAGENTA_MAGENTA));
                mvaddstr(pos->y, pos->x, "  ");
                attroff(COLOR_PAIR(MAGENTA_MAGENTA));
            }
            else
            {
                resetGridGameMap(gm);
                addPlayersToGrid(pls, gm);
                addApplesToGrid(apls, gm);

                int minHops = MIN_HOPS;

                for (int j = minHops; j > 0; j--)
                {
                    pos = scan(gm, x, y, IS_FREE, j);

                    if (pos != NULL)
                    {
                        // attron(COLOR_PAIR(YELLOW_YELLOW));
                        // mvaddstr(pos->y, pos->x, "  ");
                        // attroff(COLOR_PAIR(YELLOW_YELLOW));
                        break;
                    }
                }
            }

            if (pos != NULL)
            {
                controlPlayer(ply, pos->path);
            }
            resetGridGameMap(gm);
        }
        else
        {
            controlPlayer(ply, c);
        }

        checkSnakesForCollision(pls, gm, i);
        checkApplesForCollision(ply, apls);
        moveSnake(ply->snk, getXIncPlayer(ply), getYIncPlayer(ply));
        drawPlayer(ply);
    }
}

void checkSnakesForCollision(Player* pls[N_PLAYERS], GameMap* gm, int i)
{
    Player* ply = pls[i];
    Shape* head = ply->snk->head;
    int x       = head->x;
    int y       = head->y;
    int len     = ply->snk->len;

    if (isBorderCollision(gm, x, y)
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

        if (!ply->isDead && isCollidingWithOther(ply->snk, other->snk))
        {
            ply->isDead = 1;
        }
    }
}

void checkApplesForCollision(Player* ply, Apple* apls[N_APPLES])
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

void updateApples(Apple* apls[N_APPLES], GameMap* gm)
{
    for (int i = 0; i < N_APPLES; i++)
    {
        Apple* apl = apls[i];

        if (apl->isEaten)
        {
            while(spawnApple(apl) && IS_FREE != getGridPosition(gm, apl->shp->x, apl->shp->y));
            apl->isEaten = 0;
        }
        drawShape(apl->shp, 1, RED_RED);
        setGridPosition(gm, apl->shp->x, apl->shp->y, IS_APPLE);
    }
}

void updateBorders(Player* pls[N_PLAYERS], GameMap* gm)
{
    if (!pls[0]->isDead)
    {
        drawBorders(gm->maxX, gm->maxY, BLACK_WHITE, pls[0]->score, pls[1]->score, pls[2]->score, pls[3]->score);
    }
    else
    {
        drawBorders(gm->maxX, gm->maxY, RED_RED, pls[0]->score, pls[1]->score, pls[2]->score, pls[3]->score);
    }
}

void initPlayers(Player* pls[N_PLAYERS], GameMap* gm)
{
    pls[0] = newPlayer(gm->maxX / 30, gm->minX, gm->maxY / 2, EAST);
    pls[0]->isHuman = 1;

    pls[1] = newPlayer(gm->maxX / 30, gm->maxX - (X_INC_SNAKE * 2), gm->maxY / 2, WEST);
    pls[1]->isHuman = 0;

    // snakes have to be properly aligned with the game grid for the collision to work

    int middle = gm->maxX / 2;

    if ((gm->minX % 2 != 0 && middle % 2 == 0) || (gm->minX % 2 == 0 && middle % 2 != 0))
    {
        middle -= 1;
    }

    pls[2] = newPlayer(gm->maxX / 40, middle, gm->minY, SOUTH);
    pls[2]->isHuman = 0;

    pls[3] = newPlayer(gm->maxX / 40, middle, gm->maxY - (Y_INC_SNAKE * 2), NORTH);
    pls[3]->isHuman = 0;
}

void initApples(Apple* apls[N_APPLES], GameMap* gm)
{
    for (int i = 0; i < N_APPLES; i++)
    {
        apls[i] = newApple(gm->minX, gm->minY - 1, gm->maxX - 1, gm->maxY - 1);
        spawnApple(apls[i]);
    }
}

int isCollidingWithOther(Snake* snk1, Snake* snk2)
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

void freeApples(Apple* apls[N_APPLES])
{
    for (int i = 0; i < N_APPLES; i++)
    {
        freeApple(apls[i]);
    }
}

void freePlayers(Player* pls[N_PLAYERS])
{
    for (int i = 0; i < N_PLAYERS; i++)
    {
        Player* ply = pls[i];
        freeSnake(ply->snk);
    }
}