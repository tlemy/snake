#pragma once

#include "Shape.h"

#define NORTH 1
#define SOUTH 2
#define EAST 3
#define WEST 4

typedef struct Snake
{
    Shape* head;
    Shape* tail;
    int len;
} Snake;

Snake *newSnake(int initX, int initY, int initLen, int dirct);

Snake *growSnake(Snake* snk);

int moveSnake(Snake *snk, int diffX, int diffY);

int freeSnake(Snake *snk);
