#pragma once

#include "Snake.h"
#include "Direction.h"
#include "Key.h"

typedef struct Player
{
    Direction dir;
    int score;
    int isDead;
    int isHuman;
    int moveIdx;
    int xTarget;
    int yTarget;
    int numMoves;
    char* moves;
    Snake* snk;
} Player;

Player* newPlayer(int initLen, int x, int y, Direction dir);

int getYIncPlayer(Player* pl);

int getXIncPlayer(Player* pl);

void controlPlayer(Player* pl, Key k);

int freePlayer(Player* ply);

void drawPlayer(Player* ply);