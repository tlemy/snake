#pragma once

#include "./Snake.h"

typedef struct Player
{
    int dirct;
    int score;
    int isDead;
    int isHuman;
    int moveIdx;
    int numMoves;
    char* moves;
    Snake* snk;
} Player;

Player* newPlayer(int initLen, int x, int y, int dirct);

int getYIncPlayer(Player* pl);

int getXIncPlayer(Player* pl);

void controlPlayer(Player* pl, int c);

int freePlayer(Player* ply);

