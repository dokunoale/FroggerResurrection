#include "entities.h"

void frog(Buffer buffer, Item item) {
    while(1) {
        readItem(&buffer, &item, REVERSE_PIPE); // TODO: aggiustare
        int c;
        while ((c = getch()) == ERR) {
            usleep(USLEEP);
        }
        switch (c) {
            case 'q': item.type = EXIT; break;
            case KEY_UP:    if (item.line > 0) item.line -= 1; break;
            case KEY_DOWN:  if (item.line < GAME_HEIGHT - 1) item.line += 1; break;
            case KEY_RIGHT: if (item.column + FROG_DIM < GAME_WIDTH - 2) item.column += STEP; break;
            case KEY_LEFT:  if (item.column > 1)  item.column-= STEP; break;
        }
        writeItem(&buffer, &item, MAIN_PIPE);
    }
}

void moveFrog(Buffer* buffer, Item* item, int direction) {
    switch (direction) {
        case RIGHT: if (item->column + FROG_DIM < GAME_WIDTH - 2) item->column += STEP; break;
        case LEFT:  if (item->column > 1) item->column -= STEP; break;
    }
    writeItem(buffer, item, REVERSE_PIPE);
}

void crocodile(Buffer buffer, Item item) {
    while(1){
        switch(item.direction) {
            case RIGHT: item.column += STEP; break;
            case LEFT:  item.column -= STEP; break;
        }
        writeItem(&buffer, &item, MAIN_PIPE);
        usleep(item.speed * USLEEP);
    }
}

void bullet(Buffer buffer, Item item) {
    while(1){
        switch(item.direction) {
            case RIGHT: item.column += STEP; break;
            case LEFT:  item.column -= STEP; break;
        }
        writeItem(&buffer, &item, MAIN_PIPE);
        usleep(item.speed * USLEEP);
    }
}