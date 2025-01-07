#ifndef DISPLAY_H
#define DISPLAY_H

#include "utils.h"

#define WATER_SYM       ' '
#define FROG_SYM        'F'
#define CROCODILE_SYM   'C'

enum Colors {
    WATER_COLOR = 1,
    FROG_COLOR,
    CROCODILE_COLOR
};

void initDisplay();
void displayItem(WINDOW* win, Item* old, Item* new, Item* background);

#endif // DISPLAY_H