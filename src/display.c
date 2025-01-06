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

void mwprintarea(WINDOW *win, int start_y, int start_x, int height, int width, char text) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            mvwprintw(win, start_y + i, start_x + j, "%c", text);
        }
    }
    wrefresh(win);
}

void displayItem(WINDOW* win, Item *old, Item *new, Item *background) {
    if (old == NULL || new == NULL) { return; }

    char back_symbol = WATER_SYM;
    if (background != NULL) {
        switch (background->type) {
            case FROG: { back_symbol = FROG_SYM; } break;
            case CROCODILE: { back_symbol = CROCODILE_SYM; } break;
        }
    }

    switch (old->type) {
        case FROG: {
            mwprintarea(win, old->line * WIN_HEIGHT_RATIO, old->column, WIN_HEIGHT_RATIO, FROG_DIM, back_symbol);
            mwprintarea(win, new->line * WIN_HEIGHT_RATIO, new->column, WIN_HEIGHT_RATIO, FROG_DIM, FROG_SYM);
            mvwprintw(win, 0, 0, "Frog: %d %d", new->line, new->column);
        } break;
        case CROCODILE: {
            mwprintarea(win, old->line * WIN_HEIGHT_RATIO, old->column, WIN_HEIGHT_RATIO, CROCODILE_DIM, back_symbol);
            mwprintarea(win, new->line * WIN_HEIGHT_RATIO, new->column, WIN_HEIGHT_RATIO, CROCODILE_DIM, CROCODILE_SYM);
        } break;
    }

    // Aggiorna la finestra
    wrefresh(win);
}