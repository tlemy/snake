#include "../include/Shape.h"

#include <ncurses.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

Shape* newShape(int x, int y, char* str)
{
    Shape* shp = (Shape*) malloc(sizeof(Shape));
    shp->idx   = 0;
    shp->x     = x;
    shp->y     = y;
    shp->str   = str;
    shp->prv   = NULL;
    shp->nxt   = NULL;

    return shp;
}

Shape* growShape(Shape *shp, Shape *newShape)
{
    int idx    = shp->idx;
    Shape* lst = shp;

    lst->nxt      = newShape;
    lst->nxt->prv = lst;
    lst->nxt->idx = idx + 1;

    return lst->nxt;
}

int freeShape(Shape* shp)
{
    int numShapes = 0;

    while(shp != NULL)
    {
        Shape* tmp = shp->nxt;
        free(shp);

        shp = tmp;

        numShapes += 1;
    }
    shp = NULL;

    return numShapes;
}

int isCollidingWithPoint(Shape sh, int x, int y, int len)
{
    for (int i = 0; i < len; i++)
    {
        if (x == sh.x && y == sh.y)
        {
            return 1;
        }

        if (sh.nxt != NULL)
        {
            sh = *(sh.nxt);
        }
    }
    return 0;
}