#include "handler.h"
#include "display.h"
#include "utils.h"
#include "music.h"

int main() {

    initDisplay();
    WINDOW *main_win = newwin(WIN_MAIN_HEIGHT, WIN_MAIN_WIDTH, 0, 0); box(main_win, 0, 0); wrefresh(main_win);
    
    game();

    endwin();
    return 0;
}