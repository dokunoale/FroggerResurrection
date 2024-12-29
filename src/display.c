/**
 * @file display.c
 * @brief Contains the functions for the display of the game.
 */

#include "display.h"
#include "handler.h"

void initDisplay() {
    
    initscr();
    cbreak();
    noecho();
    curs_set(NO_CURSOR);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    clear(); refresh();
}



WINDOW* initWindow(){

    WINDOW *main_win = newwin(WIN_MAIN_HEIGHT, WIN_MAIN_WIDTH, 0, 0);
    box(main_win, 0, 0);
    wrefresh(main_win);
    return main_win;

}



void displayItem(WINDOW* main_win, Item old_entity, Item new_entity){

    switch (new_entity.type)
    {
    case FROG:

        {
            mvwaddch(main_win, old_entity.line, old_entity.column, ' ');
            mvwaddch(main_win, new_entity.line, new_entity.column, 'R');
        }
        break;

    
    case CROCODILE:
        {
            mvwaddstr(main_win, old_entity.line, old_entity.column, "   ");
            mvwaddstr(main_win, new_entity.line, new_entity.column, "CCC");
        }
        break;
    }
}

