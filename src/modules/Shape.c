#include "Shape.h"

#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

Shape* newShape(int x, int y, char* str)
{
    Shape* shp = calloc(1, sizeof(Shape));

    if (!shp)
    {
        perror("newShape");
        exit(-1);
    }

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

void drawShape(Shape *shp, int len, int pair)
{
    attron(COLOR_PAIR(pair));

    Shape* ptr = shp;

    for (int i = 0; i < len; i++)
    {
        mvaddstr(ptr->y, ptr->x, ptr->str);
        ptr = ptr->nxt;
    }

    attroff(COLOR_PAIR(pair));
}