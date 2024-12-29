#include "processes.h"

#include "display.h"

#include <unistd.h>

#include "handler.h"



int main() {

    initDisplay();
    WINDOW* main_win = initWindow();
    sleep(5);

    manche(main_win);

    endwin();
    return 0;

}