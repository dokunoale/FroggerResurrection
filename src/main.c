#include "handler.h"

int main() {

    initDisplay();
    WINDOW *main_win = newwin(WIN_MAIN_HEIGHT, WIN_MAIN_WIDTH, 0, 0); box(main_win, 0, 0); wrefresh(main_win);
    WINDOW *game_win = newwin(WIN_GAME_HEIGHT, WIN_GAME_WIDTH, 1, 1); wrefresh(game_win); wbkgd(game_win, COLOR_PAIR(WATER_COLOR)); wrefresh(game_win);

    manche(game_win);

    endwin();
    return 0;
}