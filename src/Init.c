#include <ncurses.h>

#define WHITE_WHITE 1
#define RED_RED 2
#define GREEN_GREEN 3
#define BLACK_WHITE 4

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
}

int getBorderColorPair()
{
    return WHITE_WHITE;
}

int getAppleColorPair()
{
    return RED_RED;
}

int getPlayerSnakeColorPair()
{
    return GREEN_GREEN;
}

int getTextColorPair()
{
    return BLACK_WHITE;
}