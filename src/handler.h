#ifndef HANDLER_H
#define HANDLER_H

#include "struct.h"

#define NUM_FLOWS 8
#define RIGHT 1
#define LEFT 0

#define PROBABILITY 5 // Probabilità di generare un coccodrillo 

#define CROCODILE_MAX_NUM 5

#define MIN_SPEED 8
#define MAX_SPEED 2

#define GAME_WITDH 15
#define GAME_HEIGHT 11

typedef struct{
    int id;
    int direction;
    int speed;
    Item crocodiles [CROCODILE_MAX_NUM];
    int croc_in_flow;
} Flow;

typedef struct{
    Flow flows[NUM_FLOWS];
    Item Frog;
} Game;

#endif //HANDLER_H