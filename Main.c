#include <ncurses.h>
#define CONTROL_C 3
#define SPACE ' '
#define BORDER_PAIR 1
#define APPLE_PAIR 2
#define SNAKE_PAIR 3

void drawBorders(int maxX, int maxY);
void drawTopBorder(int maxX);
void drawBottomBorder(int maxX, int maxY);
void drawLeftBorder(int maxY);
void drawRightBorder(int maxX, int maxY);

int main (void) 
{
    int maxX, maxY;

    initscr(); // start curses mode
    raw(); // disable buffering for input, keyboard interrupt passed as characters without generating a signal
    noecho(); // turn off printing the characters selected by the user
    curs_set(0); // hide cursor
    start_color(); // enable color capabilitie
    init_pair(BORDER_PAIR, COLOR_WHITE, COLOR_WHITE); // border color
    init_pair(APPLE_PAIR, COLOR_RED, COLOR_RED); // apple color
    init_pair(SNAKE_PAIR, COLOR_GREEN, COLOR_GREEN); // snake color

    /*
     * The squares being displayed take 1 row and 2 columns. ("  ")
     * As a consequence, the number of colums has to be pair.
     */
    getmaxyx(stdscr, maxY, maxX);
    maxY -= 1;
    maxX -= 1;
    if ((maxX - 1) % 2 != 0) 
    {
        maxX -= 1;
    }
    drawBorders(maxX, maxY);

    while(1) 
    {
        int c = getch();

        refresh();

        if (c == CONTROL_C) 
        {
            break;
        }
    }

    endwin(); // free resources and disable curses mode
}

void drawBorders(int maxX, int maxY)
{
    attron(COLOR_PAIR(BORDER_PAIR));
    drawTopBorder(maxX);
    drawBottomBorder(maxX, maxY);
    drawLeftBorder(maxY);
    drawRightBorder(maxX, maxY);
    attroff(COLOR_PAIR(BORDER_PAIR));
}

void drawTopBorder(int maxX) 
{
    for (int x = 0; x <= maxX ; x++) 
    {
        mvprintw(0, x, "%c", SPACE);
    }
}

void drawBottomBorder(int maxX, int maxY) 
{
    for (int x = 0; x <= maxX; x++) 
    {
        mvprintw(maxY, x, "%c", SPACE);
    }
}

void drawLeftBorder(int maxY) 
{
    for (int y = 0; y < maxY; y++) 
    {
        mvprintw(y, 0, "%c", SPACE);
    }
}

void drawRightBorder(int maxX, int maxY) 
{
    for (int y = 0; y < maxY; y++) 
    {
        mvprintw(y, maxX, "%c", SPACE);
    }
}