#ifndef HANDLER_H
#define HANDLER_H

#include "struct.h"

#define NUM_FLOWS 8
#define MAX_CROC_PER_FLOW 5
#define RIGHT 1
#define LEFT 0

#define MIN_SPEED 8
#define MAX_SPEED 2

typedef struct{
    int id;
    int direction;
    int speed;
    Item crocodiles_in_flow [MAX_CROC_PER_FLOW];
    int num_croc_in_flow;
} Flow;

typedef struct{
    Flow flows[NUM_FLOWS];
    Item Frog;
} Game;

#endif //HANDLER_H