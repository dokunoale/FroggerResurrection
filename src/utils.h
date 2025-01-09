#ifndef UTILS_H
#define UTILS_H

#include <ncurses.h>
#include <time.h>
#include "processes.h"

#define USLEEP 10000

// directions
#define RIGHT 1
#define LEFT 0

#define NUM_FLOWS 8 // Numero dei flussi
#define CROCODILE_ODDS (100) // Probabilità di generare un coccodrillo 
#define BULLET_ODDS (5) // Probabilità di generare un proiettile

#define CROCODILE_MAX_NUM 3 // Numero massimo di coccodrilli per flusso
#define BULLET_MAX_NUM 1 // Numero massimo di proiettili per flusso
#define GRANADE_MAX_NUM 2 // Numero massimo di granate per flusso - NOTA: NON MODIFICARE

#define MIN_SPEED 18 // velocità minima del flusso: influisce sulla usleep
#define MAX_SPEED 6 // velocità massima del flusso: influisce sulla usleep
#define BULLET_SPEED 5 // velocità del proiettile
#define GRANADE_SPEED (BULLET_SPEED) // velocità della granata

/**
 * @note WIN_RATIO is the ratio between the game matrix and the screen.
 * For simplicity, the ratio only works for height in game logic.
 */
#define RATIO 1
#define WIN_HEIGHT_RATIO 5 
#define WIN_WIDTH_RATIO  8 
#define STEP 3

// Item types and dimensions
typedef enum type { FROG, CROCODILE, BULLET, GRANADE, EXIT, SHOT } Type;
#define FROG_DIM        (1 * WIN_WIDTH_RATIO)
#define CROCODILE_DIM   (3 * WIN_WIDTH_RATIO)
#define BULLET_DIM      2
#define GRANADE_DIM     (BULLET_DIM)

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
    Item *crocodiles;
    Item *bullets;
    Item *granades;
} Flow;

#endif // UTILS_H