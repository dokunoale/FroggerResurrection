#include "entities.h"

void frog (Buffer buffer, Item item) {
    while (TRUE) {
        // readItem(&buffer, &item, REVERSE_PIPE);
        int c = getch();
        switch (c) {
            case 'q': item.type = EXIT; break;
            case KEY_UP: if (item.line > 1) item.line -= 1; break;
            case KEY_DOWN: if (item.line < GAME_HEIGHT) item.line += 1; break;
            case KEY_RIGHT: if (item.column < GAME_WITDH - 2) item.column += 1; break;
            case KEY_LEFT: if (item.column > 2)  item.column-= 1; break;
        }
        writeItem(&buffer, &item, MAIN_PIPE);
    }
}

void crocodile (Buffer buffer, Item item) {
    while(TRUE){
        switch(item.direction) {
            case RIGHT: item.column += 1; break;
            case LEFT:  item.column -= 1; break;
        }
        writeItem(&buffer, &item, MAIN_PIPE);
        usleep(item.speed * USLEEP);
    }
}