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

void crocodile (Buffer buffer, Item crocodile) {
    while(TRUE){
        switch(crocodile.direction) {
            case RIGHT: crocodile.column += 1; break;
            case LEFT:  crocodile.column -= 1; break;
        }
        writeItem(&buffer, &crocodile, MAIN_PIPE);
        usleep(crocodile.speed * USLEEP);
    }
}