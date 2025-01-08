#ifndef UTILS_H
#define UTILS_H

#include <ncurses.h>
#include <time.h>
#include "processes.h"

#define USLEEP 10000 * 4

// Ncurses constants
#define NO_CURSOR 0
#define CURSOR 1

// directions
#define RIGHT 1
#define LEFT 0

#define NUM_FLOWS 8 // Numero dei flussi
#define ODDS (100) // Probabilità di generare un coccodrillo 
#define CROCODILE_MAX_NUM 3 // Numero massimo di coccodrilli per flusso
#define MIN_SPEED 5
#define MAX_SPEED 1

/**
 * @note WIN_RATIO is the ratio between the game matrix and the screen.
 * For simplicity, the ratio only works for height in game logic.
 */
#define RATIO 1
#define WIN_HEIGHT_RATIO 5 // (4 * RATIO)
#define WIN_WIDTH_RATIO  8 // (6 * RATIO)
#define STEP 3

// Item types and dimensions
typedef enum type { FROG, CROCODILE, BULLET, GRANADE, EXIT } Type;
#define FROG_DIM        (1 * WIN_WIDTH_RATIO)
#define CROCODILE_DIM   (3 * WIN_WIDTH_RATIO)
#define BULLET_DIM      (1 * WIN_WIDTH_RATIO)
#define GRANADE_DIM     (1 * WIN_WIDTH_RATIO)

// Game matrix constants: WIN_RATIO only affects width
#define DEN_HEIGHT          2
#define GAME_HEIGHT         ((NUM_FLOWS) + (DEN_HEIGHT) + 1)
#define GAME_WIDTH          (15 * (WIN_WIDTH_RATIO) )
#define CROC_LEFT_START     (- (CROCODILE_DIM))
#define CROC_RIGHT_START    (GAME_WIDTH)
#define CROC_SEP            (WIN_WIDTH_RATIO)

// Screen size for windows: WIN_RATIO only affects height
#define BOX_BORDER          2
#define WIN_GAME_HEIGHT     ((GAME_HEIGHT) * WIN_HEIGHT_RATIO)
#define WIN_GAME_WIDTH      (GAME_WIDTH)
#define WIN_MAIN_HEIGHT     ((WIN_GAME_HEIGHT) + (BOX_BORDER))
#define WIN_MAIN_WIDTH      ((WIN_GAME_WIDTH) + (BOX_BORDER))

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