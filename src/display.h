#ifndef DISPLAY_H
#define DISPLAY_H

#include "processes.h"
#include <ncurses.h>

// Ncurses constants
#define NO_CURSOR 0
#define CURSOR 1

#define WATER_SYM       ' '
#define FROG_SYM        'F'
#define CROCODILE_SYM   'C'

enum colors { 
    WATER_COLOR = 100, 
    FROG_COLOR, 
    CROCODILE_COLOR, 
    BULLET_COLOR,
    DEN_COLOR,
    TEXT_COLOR,
    SCORE_COLOR,
    HEART_COLOR,
    FULL_BLACK_COLOR,
    WAVE_COLOR,
    RESERVED
};

#define NO_COLOR -1

void initDisplay();
void fill(WINDOW *win, int line, int column, int height, int width, int color);
void displayTimer(WINDOW* win);
void displayScore(WINDOW* win, int score, int line, int column);
void displayItem(WINDOW* win, Item* old, Item* new);
void displayHearts(WINDOW* win, int lifes);
void displayEnd(WINDOW* win, int status);

#endif // DISPLAY_H