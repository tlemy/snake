#pragma once

#include "./Snake.h"

typedef struct Player
{
    int dirct;
    int score;
    int isDead;
    int isHuman;
    Snake* snk;
} Player;

int getYIncPlayer(Player* pl);

int getXIncPlayer(Player* pl);

void initPlayer(Player* pl, int initLen, int x, int y, int dirct);

void controlPlayer(Player* pl, int c);

