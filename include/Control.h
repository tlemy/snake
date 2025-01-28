#pragma once
#include "./Snake.h"
#include "../include/Apple.h"

#define W_KEY 'w'
#define A_KEY 'a'
#define S_KEY 's'
#define D_KEY 'd'

typedef struct Player
{
    int dirct;
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
    int* grid;
} Limit;

int getYInc(Player* pl);

int getXInc(Player* pl);

void  initPlayer(Player* pl, Limit* lim, int x, int y, int dirct);

void initLimits(Limit* lim);

void resetGrid(Limit* lim);

void freeGrid(Limit* lim);

void controlManually(int c, Player* pl);

void controlAutomatically(Player* pl, Limit* lim);

int isBorderCollision(Snake *snk, Limit* lim);

int isAppleCollision(Snake* snk, Apple* apl);

int isCollidingWithSelf(Snake *snk, Limit* lim);

int isCollidingWithOther(Snake* snk1, Snake* snk2, Limit* lim);