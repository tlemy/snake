#pragma once
#include "./Snake.h"

typedef struct Player
{
    int left;
    int right;
    int up;
    int down;
    int score;
    Snake* snk;
    int isDead;
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

void initPlayer(Player* pl, int marginTop, Limit* lim);

void initLimits(Limit* lim);

void controlManually(int c, Player* pl);