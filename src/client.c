#include "network.h"
#include <ncurses.h>
#include <unistd.h>
#include <locale.h>

#define USLEEP 6000
#define LC_ALL 0

static const char *keys[6] = {
    "          ▁▁▁▁▁▁▁          ",
    "          ▌  ▲  ▐          ",
    "          ▔▔▔▔▔▔▔          ",
    "▁▁▁▁▁▁▁   ▁▁▁▁▁▁▁   ▁▁▁▁▁▁▁",
    "▌  ◀  ▐   ▌  ▼  ▐   ▌  ▶  ▐",
    "▔▔▔▔▔▔▔   ▔▔▔▔▔▔▔   ▔▔▔▔▔▔▔"
};

int main() {
    setlocale(LC_ALL, "");

    int client_socket = init_client();
    int data; 
    int stop_signal = 0;

    initscr();
    cbreak(); // Don't wait ENTER in getch()
    noecho(); // Hide character typed
    curs_set(0); // Remove cursor
    keypad(stdscr, TRUE); // Enable function keys listener
    nodelay(stdscr, TRUE);
    int close_remote_value = 1000;

    int row, col;
    getmaxyx(stdscr, row, col); // Get the number of rows and columns

    for (int i = 0; i < 6; i++) {
        mvprintw((row / 2 - 3) + i, (col - 27) / 2, "%s", keys[i]);
    }
    refresh();

    while(1) {
        nrecv(client_socket, &stop_signal); 
        if (stop_signal == close_remote_value) { break; }
        if ((data = getch()) == ERR) { usleep(USLEEP); continue; } 
        nsend(client_socket, &data);
    }

    endwin();
    close(client_socket);
    return 0;
}

