#ifndef UTILS_H
#define UTILS_H

#include <ncurses.h>
#include "processes.h"

#define USLEEP 100000

// Ncurses constants
#define NO_CURSOR 0
#define CURSOR 1

// Screen size for each window
#define WIN_MAIN_HEIGHT 13
#define WIN_MAIN_WIDTH 15 * 2

// directions
#define RIGHT 1
#define LEFT 0

#define NUM_FLOWS 8
#define PROBABILITY 5 // Probabilità di generare un coccodrillo 
#define CROCODILE_MAX_NUM 5
#define MIN_SPEED 8
#define MAX_SPEED 2

#define GAME_HEIGHT 12
#define GAME_WITDH 15 * 2

#define FROG 0
#define CROCODILE 1
#define BULLET 2
#define GRANADE 3
#define EXIT 10
#define FROG_DIM 1
#define CROCODILE_DIM 3

/**
 * @param line The line of the flow.
 * @param direction The direction of the flow: LEFT or RIGHT.
 * @param speed The speed of the flow.
 * @param how_many_crocodiles The number of crocodiles in the flow.
 * @param crocodiles An array of crocodiles of type Item.
 */
typedef struct Flow {
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
typedef struct Game{
    Flow flows[NUM_FLOWS];
    Item Frog;
} Game;

#endif // UTILS_H