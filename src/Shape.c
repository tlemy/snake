#include "../include/Shape.h"
#include <string.h>
#include <stdlib.h>

Unit* newUnit(int x, int y, const char* str)
{
    Unit *unt = (Unit*) malloc(sizeof(Unit));
    unt->x = x;
    unt->y = y;
    unt->str = (char*) malloc(sizeof(char) * unt->len);
    strcpy(unt->str, str);

    return unt;
}

int freeUnit(Unit* unt)
{
    free(unt->str);
    free(unt);

    return 1;
}

Shape* newShape(Unit *unt)
{
    Shape* shp = (Shape*) malloc(sizeof(Shape));
    shp->idx = 0;
    shp->unt = unt;
    shp->prv = NULL;
    shp->nxt = NULL;

    return shp;
}

Shape* addUnitToShape(Shape *shp, Unit* unt)
{
    int idx = shp->idx;
    Shape* lst = shp;

    lst->nxt = newShape(unt);
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

        if (freeUnit(shp->unt))
        {
            free(shp);
            shp = tmp;
            numShapes += 1;
        }
    }
    shp = NULL;

    return numShapes;
}