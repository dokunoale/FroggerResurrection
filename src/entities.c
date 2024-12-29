#include "entities.h"

void frog (Buffer buffer, Item item) {
    while (TRUE) {
        int c = getch();
        switch (c) {
            case 'q': item.type = EXIT; break;
            case KEY_UP: if (item.line > 1) item.line -= 1; break;
            case KEY_DOWN: if (item.line < GAME_HEIGHT - 2) item.line += 1; break;
            case KEY_RIGHT: if (item.column < GAME_WITDH - 3) item.column += 1; break;
            case KEY_LEFT: if (item.column > 2)  item.column-= 1; break;
        }
        writeItem(&buffer, &item);
    }
}

void crocodile (Buffer buffer, Item crocodile) {
    while(TRUE){
        switch(crocodile.direction) {
            case RIGHT: crocodile.column += 1; break;
            case LEFT:  crocodile.column -= 1; break;
        }
        writeItem(&buffer, &crocodile);
        usleep(crocodile.speed * USLEEP);
        break;
    }
}