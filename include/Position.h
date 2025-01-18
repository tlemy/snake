#pragma once
#include "./Snake.h"

typedef struct Player
{
    int left;
    int right;
    int up;
    int down;
    Snake* snk;
} Player;

typedef struct Limit
{
    int minX;
    int minY;
    int maxX;
    int maxY;
} Limit;

int getYInc(Player* pl);

int getXInc(Player* pl);

void initPositions(Player* pl);

void initLimits(Limit* lim);

void controlManually(int c, Player* pl);