#include "handler.h"
#include "display.h"
#include "utils.h"
#include "music.h"

int main() {
    remote();
    initDisplay();
    detectScreenSize();
    WINDOW *main_win = newwin(WIN_MAIN_HEIGHT, WIN_MAIN_WIDTH, 0, 0); wrefresh(main_win);

    wbkgd(main_win, COLOR_PAIR(DROWNED_COLOR)); 
    box(main_win, 0, 0); displayTitle(main_win); wrefresh(main_win);

    while(1) {
        switch(menu()) {
            case PLAY: wclear(main_win); box(main_win, 0, 0); wrefresh(main_win); break;
            case RECORD: showRecord(); continue;
            case QUIT: endwin(); return 0;
        }
        while(game() == 1) { wclear(main_win); box(main_win, 0, 0); wrefresh(main_win); }
    }

    close_remote();
    endwin();
    return 0;
}