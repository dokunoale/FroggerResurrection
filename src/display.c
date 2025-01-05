/**
 * @file display.c
 * @brief Contains the functions for the display of the game.
 */
#include "display.h"

void initDisplay() {
    initscr();
    cbreak();
    noecho();
    curs_set(NO_CURSOR);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    clear(); refresh();
}

void displayItem(WINDOW* win, Item *old, Item *new){
    if (old == NULL) { return; }
    switch (new->type) {
        case FROG:
            mvwaddch(win, old->line, old->column, ' ');
            mvwaddch(win, new->line, new->column, 'R');
            break;
        case CROCODILE:
            mvwaddstr(win, old->line, old->column, "   ");
            mvwaddstr(win, new->line, new->column, "CCC");
            break;
    }
    wrefresh(win);
}
