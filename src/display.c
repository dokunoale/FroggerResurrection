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
    init_pair(FROG_COLOR, COLOR_BLACK, COLOR_CYAN);
    init_pair(CROCODILE_COLOR, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(FROG_ON_CROC, COLOR_BLACK, COLOR_YELLOW);
}

void mwprintarea(WINDOW *win, int start_y, int start_x, int height, int width, int color, const char** symbol) {
    wattron(win, COLOR_PAIR(color));
    if (symbol != NULL) {
        for (int i = 1; i < height; ++i) {  
            mvwprintw(win, start_y + i, start_x, symbol[i]);
        }
    } else {
        for (int i = 1; i < height; ++i) { 
            for (int j = 0; j < width; ++j) {
                mvwaddch(win, start_y + i, start_x + j, ' ');
            }
        }
    }
    wattroff(win, COLOR_PAIR(color));
}

void displayItem(WINDOW* win, Item *old, Item *new, char background) {
    if (old == NULL || new == NULL) { return; }

    static const char *frog_sprite[] = {
        "        ",
        "▄ ▄██▄ ▄",
        "▀██▀▀██▀",
        " ▀████▀ ",
        "█▀ ▀▀ ▀█"
    };


    char color_combo = 0;
    switch (new->type) {
        case FROG: { color_combo = FROG_COLOR; } break;
        case CROCODILE: { color_combo = CROCODILE_COLOR; } break;
    }
    switch (background) {
        case CROCODILE_COLOR: { color_combo = FROG_ON_CROC; } break;
    }

    switch (old->type) {
        case FROG: {
            mwprintarea(win, old->line * WIN_HEIGHT_RATIO, old->column, WIN_HEIGHT_RATIO, FROG_DIM, background, NULL);
            mwprintarea(win, new->line * WIN_HEIGHT_RATIO, new->column, WIN_HEIGHT_RATIO, FROG_DIM, color_combo, frog_sprite);

            mvwprintw(win, 0, 0, "Frog: %d %d         ", new->line, new->column);
        } break;
        case CROCODILE: {
            mwprintarea(win, old->line * WIN_HEIGHT_RATIO, old->column, WIN_HEIGHT_RATIO, CROCODILE_DIM, background, NULL);
            mwprintarea(win, new->line * WIN_HEIGHT_RATIO, new->column, WIN_HEIGHT_RATIO, CROCODILE_DIM, color_combo, NULL);
        } break;
    }

    // Aggiorna la finestra
    wrefresh(win);
}