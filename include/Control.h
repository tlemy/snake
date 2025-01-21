#pragma once
#include "./Snake.h"
#include "../include/Apple.h"

typedef struct Player
{
    int left;
    int right;
    int up;
    int down;
    int score;
    Snake* snk;
    int isDead;
    int isHuman;
} Player;

typedef struct Limit
{
    int minX;
    int minY;
    int maxX;
    int maxY;
    int** grid;
} Limit;

int getYInc(Player* pl);

int getXInc(Player* pl);

void initPlayer(Player* pl, int marginTop, Limit* lim);

void initLimits(Limit* lim);

void resetGrid(Limit* lim);

void freeGrid(Limit* lim);

void controlManually(int c, Player* pl);

void controlAutomatically(Player* pl, Limit* lim);

int isBorderCollision(Snake *snk, Limit* lim);

int isAppleCollision(Snake* snk, Apple* apl);

int isCollidingWithSelf(Snake *snk);

int isCollidingWithOther(Snake* snk1, Snake* snk2, Limit* lim);