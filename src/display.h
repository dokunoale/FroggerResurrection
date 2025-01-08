#ifndef DISPLAY_H
#define DISPLAY_H

#include "utils.h"

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
    FROG_ON_CROC
};

void initDisplay();
void displayItem(WINDOW* win, Item* old, Item* new, char background);

#endif // DISPLAY_H