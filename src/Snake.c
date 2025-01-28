#include "../include/Snake.h"
#include <stdlib.h>
#include <stdio.h>

Snake *newSnake(int initX, int initY, int initLen, int dirct)
{
    Shape* head = newShape(newUnit(initX, initY, SQUARE));
    Shape* tail = head;
    int i = 1;
    int xModifier = 0;
    int yModifier = 0;

    if (dirct == NORTH)
    {
        yModifier = -Y_INC_SNAKE;
    }
    else if (dirct == SOUTH)
    {
        yModifier = Y_INC_SNAKE;
    }
    else if (dirct == EAST)
    {
        xModifier = X_INC_SNAKE;
    }
    else if (dirct == WEST)
    {
        xModifier = -X_INC_SNAKE;
    }

    for (i = 1; i < initLen; i++)
    {
        tail = addUnitToShape(tail, newUnit(tail->unt->x - xModifier, tail->unt->y - yModifier, SQUARE));
    }

    Snake *snk = (Snake*) malloc(sizeof(Snake));
    snk->head = head;
    snk->tail = tail;
    snk->len = i;

    return snk;
}

Snake *growSnake(Snake* snk)
{
    snk->tail = addUnitToShape(snk->tail, newUnit(-1, -1, SQUARE)); // (-1, -1) is out of the viewport
    snk->len += 1;

    return snk;
}

int moveSnake(Snake *snk, int diffX, int diffY)
{
    int i = 1;
    Shape* ptr = snk->tail;

    for (i = 1; i < snk->len; i++)
    {
        ptr->unt->x = ptr->prv->unt->x;
        ptr->unt->y = ptr->prv->unt->y;

        ptr = ptr->prv;
    }

    snk->head->unt->x += diffX;
    snk->head->unt->y += diffY;

    return i;
}

int freeSnake(Snake *snk)
{
    int i = 0;

    if (freeShape(snk->head) == snk->len)
    {
        free(snk);
        return 1;
    }

    return 0;
}