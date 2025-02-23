#pragma once

#include "Shape.h"

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
