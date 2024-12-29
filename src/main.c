#include "handler.h"

int main() {

    initDisplay();
    // WINDOW *main_win = newwin(WIN_MAIN_HEIGHT, WIN_MAIN_WIDTH, 0, 0);
    WINDOW *game_win = newwin(WIN_MAIN_HEIGHT - 1, WIN_MAIN_WIDTH - 1, 1, 1);
    box(game_win, 0, 0); // todo: rimuovere
    wrefresh(game_win);

    manche(game_win);

    endwin();
    return 0;

}