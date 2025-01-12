#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

#define USLEEP 6000 

// directions
#define RIGHT 1
#define LEFT 0

#define LIFES 5 // Numero di vite iniziali
#define BASE_SCORE 100

#define NUM_FLOWS 8 // Numero dei flussi
#define CROCODILE_ODDS (100) // Probabilità di generare un coccodrillo 
#define BULLET_ODDS (5) // Probabilità di generare un proiettile

#define CROCODILE_MAX_NUM 3 // Numero massimo di coccodrilli per flusso
#define BULLET_MAX_NUM 1 // Numero massimo di proiettili per flusso
#define GRANADE_MAX_NUM 2 // Numero massimo di granate per flusso - NOTA: NON MODIFICARE
#define DEN_NUM 5 // Numero di tane

#define MIN_SPEED 10 // velocità minima del flusso: influisce sulla usleep
#define MAX_SPEED 6 // velocità massima del flusso: influisce sulla usleep
#define BULLET_SPEED 4 // velocità del proiettile
#define GRANADE_SPEED (BULLET_SPEED) // velocità della granata
#define TIMER_SPEED 60 // velocità del timer

/**
 * @note WIN_RATIO is the ratio between the game matrix and the screen.
 * For simplicity, the ratio only works for height in game logic.
 */
#define RATIO 1
#define WIN_HEIGHT_RATIO 5 
#define WIN_WIDTH_RATIO  8 
#define STEP 1
#define FROG_STEP 3

// Item types and dimensions
typedef enum type { LOSE = 0, WIN, FROG, CROCODILE, BULLET, GRANADE, SHOT, DEN, HEART, TIMER, EXIT } Type;
#define FROG_DIM        (1 * WIN_WIDTH_RATIO)
#define CROCODILE_DIM   (3 * WIN_WIDTH_RATIO)
#define BULLET_DIM      2
#define GRANADE_DIM     (BULLET_DIM)
#define HEART_DIM       3

// Game matrix constants: WIN_RATIO only affects width
#define DEN_HEIGHT          2
#define GAME_HEIGHT         ((NUM_FLOWS) + (DEN_HEIGHT) + 1)
#define GAME_WIDTH          (15 * (WIN_WIDTH_RATIO) )
#define CROC_LEFT_START     (- (CROCODILE_DIM))
#define CROC_RIGHT_START    (GAME_WIDTH)
#define CROC_SEP            (WIN_WIDTH_RATIO)
#define TIMER_START         (GAME_WIDTH)

// Screen size for windows: WIN_RATIO only affects height
#define BOX_BORDER          2
#define WIN_GAME_HEIGHT     ((GAME_HEIGHT) * WIN_HEIGHT_RATIO)
#define WIN_GAME_WIDTH      (15 * WIN_WIDTH_RATIO)
#define WIN_TIMER_HEIGHT    (WIN_HEIGHT_RATIO)
#define WIN_TIMER_WIDTH     (WIN_GAME_WIDTH)
#define WIN_INFO_HEIGHT     (WIN_HEIGHT_RATIO - 1)
#define WIN_INFO_WIDTH      (WIN_GAME_WIDTH) 
#define WIN_MAIN_HEIGHT     ((WIN_GAME_HEIGHT) + (WIN_INFO_HEIGHT) + (WIN_TIMER_HEIGHT) + (BOX_BORDER))
#define WIN_MAIN_WIDTH      ((WIN_GAME_WIDTH) + (BOX_BORDER))
#define WIN_END_HEIGHT      (2 * WIN_HEIGHT_RATIO)
#define WIN_END_WIDTH       (6 * WIN_WIDTH_RATIO)

#define SCORE_INFO          (WIN_INFO_WIDTH - 20)
#define HEART_INFO          (5)
#define STAGE_NUM           8
#define STAGE_SKIP          3

typedef enum settings { PLAY = 0, RECORD, QUIT } Menu;
#define setline(x) (settings_line + (x * settings_height))

#endif // UTILS_H