#ifndef DISPLAY_H
#define DISPLAY_H

#include "entities.h"

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
    TEXT,
    RESERVED
};

void initDisplay();
void displayTimer(WINDOW* win);
void displayScore(WINDOW* win, int score);
void displayItem(WINDOW* win, Item* old, Item* new);

#endif // DISPLAY_H