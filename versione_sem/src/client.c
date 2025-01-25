#include "network.h"
#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>
#include <locale.h>

#define USLEEP 6000

int client_socket;

int main() {
    setlocale(LC_ALL, "");

    client_socket = init_client();
    int data; 

    
    
    initscr();
    cbreak(); // Don't wait ENTER in getch()
    noecho(); // Hide character typed
    curs_set(0); // Remove cursor
    keypad(stdscr, TRUE); // Enable function keys listener
    nodelay(stdscr, TRUE); refresh();

    mvprintw(0, 0, "The client successfully connected\n");

    while(1) {
        if ((data = getch()) == ERR) { usleep(USLEEP); continue; } 
        nsend(client_socket, &data);
    }

    clear(); refresh(); endwin();
    close(client_socket);
    return 0;
}