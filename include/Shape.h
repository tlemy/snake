#pragma once

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