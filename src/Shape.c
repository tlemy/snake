#include "../include/Shape.h"
#include <string.h>

Unit* newUnit(int x, int y, char* str) 
{
    Unit *unt = (Unit*) malloc(sizeof(Unit));
    unt->x = x;
    unt->y = y;
    unt->len = strlen(str);
    unt->str = (char*) malloc(sizeof(char) * unt->len);

    return unt;
}

int freeUnit(Unit* unt) 
{
    free(unt->str);
    unt->len = 0;
    unt->str = NULL;

    return 0;
}

Shape* newShape() 
{
    Shape* shp = (Shape*) malloc(sizeof(Shape));
    shp->idx = 0;
    shp->unt = NULL;
    shp->nxt = NULL;
}

int freeShape(Shape* shp) 
{
    int lastIdx = 0;

    while(shp != NULL) 
    {
        Unit *tmp = shp->nxt;
        freeUnit(shp->unt);
        free(shp);
        shp = tmp;
        lastIdx += 1;
    }

    return lastIdx;
}

int addUnitToShape(Shape *shp, Unit* unt) 
{
    Shape* last = shp;

    while (last->nxt != NULL) 
    {
        last = last->nxt;
    }

    Shape *nxt = newShape();
    nxt->idx = shp->idx + 1;
    nxt->unt = unt;
}