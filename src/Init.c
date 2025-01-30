#include <ncurses.h>

#include "../include/Init.h"

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
        mvaddch(0, x, BLOC);
    }

    // bottom
    for (int x = 0; x <= maxX; x++)
    {
        mvaddch(maxY, x, BLOC);
    }

    // left
    for (int y = 0; y < maxY; y++)
    {
        mvaddch(y, 0, BLOC);
    }

    // right
    for (int y = 0; y < maxY; y++)
    {
        mvaddch(y, maxX, BLOC);
    }

    attroff(COLOR_PAIR(pair));
}