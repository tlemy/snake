#include <ncurses.h>
#include <string.h>
#include "../include/Shape.h"
#include "../include/Init.h"

#define SPACE ' '

void drawShape(Shape *shp, int len, int pair)
{
    attron(COLOR_PAIR(pair));

    Shape* ptr = shp;

    for (int i = 0; i < len; i++)
    {
        mvaddstr(ptr->unt->y, ptr->unt->x, ptr->unt->str);
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