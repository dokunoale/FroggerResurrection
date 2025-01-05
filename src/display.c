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

void displayItem(WINDOW* win, Item *old, Item *new) {
    if (old == NULL || new == NULL) { 
        return; 
    }

    // Cancella la rappresentazione precedente
    switch (old->type) {
        case FROG:
            mvwaddch(win, old->line, old->column, ' ');
            break;
        case CROCODILE:
            for (int i = 0; i < CROCODILE_DIM; i++) {
                if (old->column + i < GAME_WIDTH && old->line < GAME_HEIGHT) {
                    mvwaddch(win, old->line, old->column + i, ' ');
                }
            }
            break;
    }

    // Disegna la nuova rappresentazione
    switch (new->type) {
        case FROG:
            mvwaddch(win, new->line, new->column, 'R');
            break;
        case CROCODILE:
            for (int i = 0; i < CROCODILE_DIM; i++) {
                if (new->column + i < GAME_WIDTH && new->line < GAME_HEIGHT) {
                    mvwaddch(win, new->line, new->column + i, 'C');
                }
            }
            break;
    }

    // Aggiorna la finestra
    wrefresh(win);
}
