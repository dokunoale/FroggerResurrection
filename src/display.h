#ifndef DISPLAY_H
#define DISPLAY_H

// Ncurses constants
#define NO_CURSOR 0
#define CURSOR 1

// Screen size for each window
#define WIN_MAIN_HEIGHT 11
#define WIN_MAIN_WIDTH 15 * 2

#include <ncurses.h>
#include "display.h"
#include "processes.h"
#include "handler.h"

void initDisplay();
WINDOW* initWindow();
void displayItem(WINDOW* main_win, Item old_item, Item new_item);


#endif // DISPLAY_H