#include <ncurses.h>
#include <unistd.h>
#include "../include/Snake.h"
#include "../include/Apple.h"
#include <string.h>

#define CONTROL_C_KEY 3
#define LINE_FEED_KEY 10
#define CARRIAGE_RETURN_KEY 13
#define W_KEY 'w'
#define A_KEY 'a'
#define S_KEY 's'
#define D_KEY 'd'
#define SPACE ' '
#define WHITE_WHITE 1
#define RED_RED 2
#define GREEN_GREEN 3
#define BLACK_WHITE 4
#define X_INC_SNAKE 2
#define Y_INC_SNAKE 1
#define SLEEP_TIME 50 // millisecs
 
void setup(void);
void initPositions(int *left, int *right, int *up, int *down);
void initLimits(int *minX, int *minY, int *maxX, int *maxY);
int isBorderCollision(Snake *snk, int minX, int minY, int maxX, int maxY);
int isAppleCollision(Snake* snk, Apple* apl);
void movementControls(int c, int *left, int *right, int *up, int *down);
int getYInc(int up, int down);
int getXInc(int left, int right);
void drawShape(Shape *shp, int len, int pair);
void drawBorders(int maxX, int maxY, int pair, int scr);

int main (void) 
{
    setup();

    int gameOver = false;

    int left;
    int right;
    int up;
    int down;
    initPositions(&left, &right, &up, &down);

    int minX;
    int minY;
    int maxX;
    int maxY;
    initLimits(&minX, &minY, &maxX, &maxY);

    int score = 0;
    Snake* snk = newSnake(minX, minY, maxX / 10);
    Apple* apl = newApple(minX, minY, maxX - 2, maxY -1);

    spawnApple(apl);

    while(1) 
    {
        // controls
        int c = getch();

        if (CONTROL_C_KEY == c) 
        {   
            freeSnake(snk);
            freeApple(apl);
            endwin(); // free resources and disable curses mode
            break;
        }

        if (!gameOver)
        {
            movementControls(c, &left, &right, &up, &down); 
        }
        else 
        {
            if (CARRIAGE_RETURN_KEY == c || LINE_FEED_KEY == c) 
            {
                gameOver = false;
                initPositions(&left, &right, &up, &down);
                initLimits(&minX, &minY, &maxX, &maxY);
                snk = newSnake(minX, minY, maxX / 10);
                score = 0;
            }
        }

        // movement
        if (!gameOver) 
        {
            int xInc = getXInc(left, right);
            int yInc = getYInc(up, down);
            moveSnake(snk, xInc, yInc);
        }

        // collision
        if (!gameOver && (isBorderCollision(snk, minX, minY, maxX, maxY) 
                || isCollidingWithSelf(snk))) 
        {
            gameOver = true;
        }

        if (isAppleCollision(snk, apl)) 
        {
            growSnake(snk);
            spawnApple(apl);
            ++score;
        }

        // display
        if (!gameOver) 
        {
            erase();
            drawBorders(maxX, maxY, WHITE_WHITE, score);
            drawShape(snk->head, snk->len, GREEN_GREEN);
            drawShape(apl->shp, 1, RED_RED);
            refresh();
        }
        else 
        {
            drawBorders(maxX, maxY, RED_RED, score);
        }
        napms(1000 / 20);
    } 
}

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

void initPositions(int *left, int *right, int *up, int *down) 
{
    *left = 0;
    *right = 1;
    *up = 0;
    *down = 0;
}

void initLimits(int *minX, int *minY, int *maxX, int *maxY) 
{
    *minX = 1;
    *minY = 1;

    getmaxyx(stdscr, *maxY, *maxX);
    *maxY -= 1; // rows start at idx 0
    *maxX -= 1; // columns start at idx 0

    /*
    * The squares being displayed take 1 row and 2 columns. ("  ")
    * As a consequence, the number of colums has to be pair.
    */
    if ((*maxX - 1) % 2 != 0) 
    {
        *maxX -= 1;
    }
}

int isBorderCollision(Snake *snk, int minX, int minY, int maxX, int maxY) 
{
    int leftBorderCollision = snk->head->unt->x < minX;
    int topBorderCollision = snk->head->unt->y < minY;
    int rightBorderCollision = snk->head->unt->x > maxX - 2; // each unit is 2 characters long
    int bottomBorderCollision = snk->head->unt->y > maxY - 1; // each unit is one character high

    return leftBorderCollision || rightBorderCollision || topBorderCollision || bottomBorderCollision;
}

int isAppleCollision(Snake* snk, Apple* apl) 
{
    int isXEqual = snk->head->unt->x == apl->shp->unt->x;
    int isYEqual = snk->head->unt->y == apl->shp->unt->y;

    return isXEqual && isYEqual;
}

void movementControls(int c, int *left, int *right, int *up, int *down) 
{
    if (c == W_KEY && !(*down)) 
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