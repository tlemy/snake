#pragma once
#include "../include/Snake.h"
#include "../include/Apple.h"

int isBorderCollision(Snake *snk, int minX, int minY, int maxX, int maxY);

int isAppleCollision(Snake* snk, Apple* apl);