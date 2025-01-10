#include "display.h"
#include "utils.h"

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
    init_pair(DEN_COLOR, COLOR_GREEN, COLOR_GREEN);
    init_pair(TEXT, COLOR_WHITE, COLOR_BLACK);
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

void displayTimer(WINDOW* win) {
    fill(win, 1, 1, 3, WIN_GAME_WIDTH - 2, CROCODILE_COLOR);
}

void displayScore(WINDOW* win, int score) {
    wattron(win, COLOR_PAIR(TEXT));
    mvwprintw(win, 1, WIN_GAME_WIDTH - 20, "Score: %d", score);
    wrefresh(win);
    wattroff(win, COLOR_PAIR(TEXT));
}

void displayItem(WINDOW* win, Item *old, Item *new) {
    if (old == NULL || new == NULL) { return; }
    
    switch (old->type) {
        case FROG: { // mvwprintw(win, new->line * WIN_HEIGHT_RATIO, 1, "Frog: %d  ", new->column);
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
        case GRANADE: {
            fill(win, old->line * WIN_HEIGHT_RATIO + 2, old->column, 1, GRANADE_DIM, WATER_COLOR);
            fill(win, new->line * WIN_HEIGHT_RATIO + 2, new->column, 1, GRANADE_DIM, FROG_COLOR);
        } break;
        case DEN: {
            fill(win, old->line * WIN_HEIGHT_RATIO, old->column, WIN_HEIGHT_RATIO, FROG_DIM, WATER_COLOR);
            fill(win, new->line * WIN_HEIGHT_RATIO + 1, new->column, WIN_HEIGHT_RATIO - 1, FROG_DIM, DEN_COLOR);
        } break;
        case HEART: {
            fill(win, new->line * WIN_HEIGHT_RATIO + 1, new->column, WIN_HEIGHT_RATIO - 3, HEART_DIM, BULLET_COLOR);
        } break;
        case TIMER: {
            fill(win, new->line * WIN_HEIGHT_RATIO, new->column, WIN_HEIGHT_RATIO, FROG_DIM, FROG_COLOR);
        } break;
    }

    // Aggiorna la finestra
    wrefresh(win);
}