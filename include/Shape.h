#pragma once

#define BLOC ' '
#define SQUARE "  "

#define X_INC_SNAKE 2
#define Y_INC_SNAKE 1

#define NORTH 1
#define SOUTH 2
#define EAST 3
#define WEST 4

#define W_KEY 'w'
#define A_KEY 'a'
#define S_KEY 's'
#define D_KEY 'd'

struct Shape;
typedef struct Shape
{
    int idx;
    int x;
    int y;
    char *str;
    struct Shape* prv;
    struct Shape* nxt;
} Shape;

Shape* newShape(int x, int y, char* str);

Shape* growShape(Shape *shp, Shape *newShape);

int freeShape(Shape* shp);

int isCollidingWithPoint(Shape sh, int x, int y, int len);