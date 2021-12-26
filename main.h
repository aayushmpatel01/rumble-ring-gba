#ifndef MAIN_H
#define MAIN_H

#include "gba.h"

#define P_HEIGHT 25
#define P_WIDTH 15

#define O_HEIGHT 12
#define O_WIDTH 7

int points = 0;

int timer = 0;

// TODO: Create any necessary structs


struct player1 {
int row;
int col;
int rowDisp; //vertical velocity
int colDisp; //horizontal velocity
};

struct opponent { //struct for opposition fighters
    int row;
    int col;
    int alive;
    int move;
};

struct state {
int currentState;
int nextState;
};

#endif
