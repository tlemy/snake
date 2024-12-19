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
    struct Shape* prv;
    struct Shape* nxt;
} Shape;

Unit* newUnit(int x, int y, const char* str);
int freeUnit(Unit *unt);
Shape* newShape(Unit *unt);
Shape* addUnitToShape(Shape *shp, Unit* unt);
int freeShape(Shape* shp);