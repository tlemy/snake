#pragma once

#include <string.h>

#include "./Shape.h"

#define WHITE_WHITE 1
#define RED_RED 2
#define GREEN_GREEN 3
#define BLACK_WHITE 4
#define BLUE_BLUE 5

void setup(void);

void drawShape(Shape *shp, int len, int pair);

void drawBorders(int maxX, int maxY, int pair, int scr);