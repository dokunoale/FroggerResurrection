#ifndef DISPLAY_H
#define DISPLAY_H

#include "utils.h"

// Ncurses constants
#define NO_CURSOR 0
#define CURSOR 1

// Screen size for each window
#define WIN_MAIN_HEIGHT 11
#define WIN_MAIN_WIDTH 15 * 2

void initDisplay();
void displayItem(WINDOW *win, Item *old, Item *new);


#endif // DISPLAY_H