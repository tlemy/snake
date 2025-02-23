#pragma once

#include "Snake.h"
#include "Direction.h"
#include "Key.h"

typedef struct Player
{
    Direction dir;
    int score;
    Snake* snk;
    int isDead;
    int isHuman;
} Player;

Player* newPlayer(int initLen, int x, int y, Direction dir);

int getYIncPlayer(Player* pl);

int getXIncPlayer(Player* pl);

void controlPlayer(Player* pl, Key k);

int freePlayer(Player* ply);

void drawPlayer(Player* ply);