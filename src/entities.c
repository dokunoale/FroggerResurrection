#include "entities.h"

void frog (Buffer buffer, Item item) {
    while (TRUE) {
        readItem(&buffer, &item, REVERSE_PIPE);
        int c;
        while ((c = getch()) == ERR) {
            usleep(USLEEP);
        }
        switch (c) {
            case 'q': item.type = EXIT; break;
            case KEY_UP: if (item.line > 1) item.line -= 1; break;
            case KEY_DOWN: if (item.line < GAME_HEIGHT - 1) item.line += 1; break;
            case KEY_RIGHT: if (item.column < GAME_WIDTH - 2) item.column += STEP; break;
            case KEY_LEFT: if (item.column > 1)  item.column-= STEP; break;
        }
        writeItem(&buffer, &item, MAIN_PIPE);
    }
}

void crocodile (Buffer buffer, Item crocodile) {
    Item item = crocodile;
    while(TRUE){
        switch(item.direction) {
            case RIGHT: item.column += STEP; break;
            case LEFT:  item.column -= STEP; break;
        }
        writeItem(&buffer, &item, MAIN_PIPE);
        usleep(item.speed * USLEEP);
    }
}