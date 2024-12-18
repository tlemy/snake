#define SQUARE "  "

typedef struct Unit
{
    int x;
    int y;
    int len;
    char *str;
} Unit;

struct Shape;
typedef struct Shape 
{
    int idx;
    Unit* unt;
    struct Shape* nxt;
} Shape;

Unit* newUnit(int x, int y, char* str);
int freeUnit(Unit *unt);
Shape* newShape();
int freeShape(Shape* shp);
int addUnitToShape(Shape *shp, Unit* unt);