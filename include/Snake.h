#ifndef SNAKE
#define SNAKE
#include "Shape.h"

typedef struct Snake 
{
    Shape* head;
    Shape* tail;
    int len;
} Snake;

Snake *newSnake(int initX, int initY, int initLen);
Snake *growSnake(Snake* snk);
int moveSnake(Snake *snk, int diffX, int diffY);
int freeSnake(Snake *snk);
int isCollidingWithSelf(Snake *snk);

#endif
