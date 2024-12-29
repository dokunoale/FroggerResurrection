#include "processes.h"
#include "display.h"
#include <unistd.h>
#include "handler.h"

int main() {

    initDisplay();
    // WINDOW *main_win = newwin(WIN_MAIN_HEIGHT, WIN_MAIN_WIDTH, 0, 0);
    WINDOW *game_win = newwin(WIN_MAIN_HEIGHT, WIN_MAIN_WIDTH, 0, 0);
    box(game_win, 0, 0); // todo: rimuovere
    sleep(5);

    manche(game_win);

    endwin();
    return 0;

}