#pragma once
#include "Shape.h"

typedef struct Apple
{
    Shape *shp;
    int minX;
    int minY;
    int maxX;
    int maxY;
    int isEaten;
} Apple;

void initApple(Apple* apl, int minX, int minY, int maxX, int maxY);

int spawnApple(Apple *apl);

int freeApple(Apple *apl);