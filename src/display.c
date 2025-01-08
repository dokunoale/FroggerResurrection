/**
 * @file display.c
 * @brief Contains the functions for the display of the game.
 */
#include "display.h"
#include <locale.h>

void initDisplay() {
    setlocale(LC_ALL, "");

    initscr();
    cbreak();
    noecho();
    curs_set(NO_CURSOR);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    clear(); refresh();

    start_color();
    init_pair(WATER_COLOR, COLOR_CYAN, COLOR_CYAN);
    init_pair(FROG_COLOR, COLOR_BLACK, COLOR_BLACK);
    init_pair(CROCODILE_COLOR, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(BULLET_COLOR, COLOR_RED, COLOR_RED);
}

/*
static const char *frog_sprite[] = {
    "        ",
    "▄ ▄██▄ ▄",
    "▀██▀▀██▀",
    " ▀████▀ ",
    "█▀ ▀▀ ▀█"
};
*/

void fill(WINDOW *win, int line, int column, int height, int width, int color) {
    wattron(win, COLOR_PAIR(color));
    for (int i = 0; i < height; ++i) { 
        for (int j = 0; j < width; ++j) {
            mvwaddch(win, line + i, column + j, ' ');
        }
    }
    wattroff(win, COLOR_PAIR(color));
}

void displayItem(WINDOW* win, Item *old, Item *new) {
    if (old == NULL || new == NULL) { return; }
    
    switch (old->type) {
        case FROG: { mvwprintw(win, 0, 0, "Frog: %d %d         ", new->line, new->column);
            fill(win, old->line * WIN_HEIGHT_RATIO, old->column, WIN_HEIGHT_RATIO, FROG_DIM, WATER_COLOR);
            fill(win, new->line * WIN_HEIGHT_RATIO + 1, new->column, WIN_HEIGHT_RATIO - 1, FROG_DIM, FROG_COLOR);
        } break;
        case CROCODILE: {
            fill(win, old->line * WIN_HEIGHT_RATIO, old->column, WIN_HEIGHT_RATIO, CROCODILE_DIM, WATER_COLOR);
            fill(win, new->line * WIN_HEIGHT_RATIO + 1, new->column, WIN_HEIGHT_RATIO - 1, CROCODILE_DIM, CROCODILE_COLOR);
        } break;
        case BULLET: {
            fill(win, old->line * WIN_HEIGHT_RATIO + 2, old->column, 1, BULLET_DIM, WATER_COLOR);
            fill(win, new->line * WIN_HEIGHT_RATIO + 2, new->column, 1, BULLET_DIM, BULLET_COLOR);
        } break;
    }

    // Aggiorna la finestra
    wrefresh(win);
}