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

/**
 * @param line The line of the flow.
 * @param direction The direction of the flow: LEFT or RIGHT.
 * @param speed The speed of the flow.
 * @param how_many_crocodiles The number of crocodiles in the flow.
 * @param crocodiles An array of crocodiles of type Item.
 */
typedef struct {
    unsigned int line;
    unsigned int direction;
    unsigned int speed;
    unsigned int how_many_crocodiles;
    Item crocodiles[CROCODILE_MAX_NUM];
} Flow;

/**
 * @param flow An array of flows.
 * @param frog The frog item.
 */
typedef struct {
    Flow flows[NUM_FLOWS];
    Item Frog;
} Game;

void manche(WINDOW *game_win);

#endif //HANDLER_H