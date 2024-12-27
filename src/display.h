#ifndef DISPLAY_H
#define DISPLAY_H

// Ncurses constants
#define NO_CURSOR 0
#define CURSOR 1
#define NO_CURSOR 0

// Screen size for each window
#define WIN_MAIN_HEIGHT 11
#define WIN_MAIN_WIDTH 15 * 2

#include <ncurses.h>

void initDisplay();

#endif // DISPLAY_H