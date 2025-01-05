#ifndef UTILS_H
#define UTILS_H

#include <ncurses.h>
#include <time.h>
#include "processes.h"

#define USLEEP 10000 * 10

// Ncurses constants
#define NO_CURSOR 0
#define CURSOR 1

// directions
#define RIGHT 1
#define LEFT 0

#define NUM_FLOWS 8
#define ODDS USLEEP // Probabilità di generare un coccodrillo 
#define CROCODILE_MAX_NUM 3 // Numero massimo di coccodrilli per flusso
#define MIN_SPEED 4
#define MAX_SPEED 4

#define BOX_BORDER 2
#define DEN_HEIGHT 2
#define GAME_HEIGHT NUM_FLOWS + DEN_HEIGHT + 1
#define GAME_WIDTH 15 * 2
#define CROC_LEFT_START - CROCODILE_DIM
#define CROC_RIGHT_START GAME_WIDTH

// Screen size for windows
#define WIN_GAME_HEIGHT GAME_HEIGHT
#define WIN_GAME_WIDTH GAME_WIDTH
#define WIN_MAIN_HEIGHT GAME_HEIGHT + BOX_BORDER
#define WIN_MAIN_WIDTH GAME_WIDTH + BOX_BORDER

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
    Item *crocodiles;
} Flow;

#endif // UTILS_H