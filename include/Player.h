#pragma once

#include "./Snake.h"

#define W_KEY 'w'
#define A_KEY 'a'
#define S_KEY 's'
#define D_KEY 'd'

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

int getYIncPlayer(Player* pl);

int getXIncPlayer(Player* pl);

void initPlayer(Player* pl, int initLen, int x, int y, int dirct);

void controlPlayer(Player* pl, int c);

