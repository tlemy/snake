#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include "../include/Snake.h"

#define CONTROL_C_KEY 3
#define W_KEY 'w'
#define A_KEY 'a'
#define S_KEY 's'
#define D_KEY 'd'
#define SPACE ' '
#define BORDER_PAIR 1
#define APPLE_PAIR 2
#define SNAKE_PAIR 3
#define SCORE_PAIR 4
#define X_INC_SNAKE 2
#define Y_INC_SNAKE 1
#define SLEEP_TIME 50 // millisecs
 
void setup(void);
int manageControls(int c, int *left, int *right, int *up, int *down);
int getYInc(int up, int down);
int getXInc(int left, int right);
void drawShape(Shape *shp, int len, int pair);
void drawBorders(int maxX, int maxY);

int main (void) 
{
    setup();

    int left = 0;
    int right = 1;
    int up = 0;
    int down = 0;
    int maxX, maxY;
    getmaxyx(stdscr, maxY, maxX);
    maxY -= 1; // rows start at idx 0
    maxX -= 1; // columns start at idx 0

    Snake* snk = newSnake(1, 1, maxX / 10);

    /*
     * The squares being displayed take 1 row and 2 columns. ("  ")
     * As a consequence, the number of colums has to be pair.
     */
    if ((maxX - 1) % 2 != 0) 
    {
        maxX -= 1;
    }

    while(1) 
    {
        int c = getch();
        int xInc = getXInc(left, right);
        int yInc = getYInc(up, down);

        if (manageControls(c, &left, &right, &up, &down) == -1) 
        {
            break;
        }

        moveSnake(snk, xInc, yInc);

        erase();
        drawBorders(maxX, maxY);
        drawShape(snk->head, snk->len, SNAKE_PAIR);
        refresh();

        napms(1000 / 20);
    }

    if (!freeSnake(snk)) 
    {
        return -1;
    }
    endwin(); // free resources and disable curses mode
}

void setup(void) 
{
    initscr(); // start curses mode
    raw(); // disable buffering for input, keyboard interrupt passed as characters without generating a signal
    noecho(); // turn off printing the characters selected by the user
    curs_set(0); // hide cursor
    start_color(); // enable color capabilitie
    nodelay(stdscr, TRUE); // avoid getch blocking
    init_pair(BORDER_PAIR, COLOR_WHITE, COLOR_WHITE); // border color
    init_pair(APPLE_PAIR, COLOR_RED, COLOR_RED); // apple color
    init_pair(SNAKE_PAIR, COLOR_GREEN, COLOR_GREEN); // snake color
    init_pair(SCORE_PAIR, COLOR_BLACK, COLOR_WHITE); // score color
}

int manageControls(int c, int *left, int *right, int *up, int *down) 
{
    if (c == CONTROL_C_KEY) 
    {
        return -1;
    }
    else if (c == W_KEY && !(*down)) 
    {
        *up = 1;
        *down = 0;
        *left = 0;
        *right = 0;
    }
    else if (c == S_KEY && !(*up)) 
    {
        *down = 1;
        *up = 0;
        *left = 0;
        *right = 0;
    }
    else if (c == A_KEY && !(*right)) 
    {
        *left = 1;
        *right = 0;
        *up = 0;
        *down = 0;
    }
    else if (c == D_KEY && !(*left))
    {
        *right = 1;
        *left = 0;
        *up = 0;
        *down = 0;
    }
    return 0;
}

int getYInc(int up, int down) 
{
    if (up) 
    {
        return -1 * Y_INC_SNAKE;
    }
    else if (down) 
    {
        return Y_INC_SNAKE;
    }
    return 0;
}

int getXInc(int left, int right) 
{
    if (left) 
    {
        return -1 * X_INC_SNAKE;
    }
    else if (right) 
    {
        return X_INC_SNAKE;
    }
    return 0;
}

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

void drawBorders(int maxX, int maxY)
{
    attron(COLOR_PAIR(BORDER_PAIR));

    // top
    for (int x = 0; x <= maxX ; x++) 
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

    attroff(COLOR_PAIR(BORDER_PAIR));
}