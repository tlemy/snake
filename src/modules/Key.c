#include "Key.h"
#include "Direction.h"

char translateDirectionToKey(int dir)
{
    switch (dir)
    {
    case NORTH:
        return NORTH_KEY;
    case SOUTH:
        return SOUTH_KEY;
    case WEST:
        return WEST_KEY;
    case EAST:
        return EAST_KEY;
    default:
        return '\0';
    }
}