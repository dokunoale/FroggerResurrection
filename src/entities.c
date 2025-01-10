#include "entities.h"
#include "utils.h"
#include "display.h"

void frog(Buffer buffer, Item item) {
    writeItem(&buffer, &item, MAIN_PIPE);
    while(1) {
        item.type = FROG;
        int c, size = readItem(&buffer, &item, REVERSE_PIPE);
        if (size != 0) { writeItem(&buffer, &item, MAIN_PIPE); continue; } 
        if ((c = getch()) == ERR) { usleep(USLEEP); continue; }
        switch (c) {
            case 'q': item.type = EXIT; break;
            case ' ': item.type = SHOT; break;
            case 'W':; case 'w':;
            case KEY_UP:    if (item.line > 0) item.line -= 1; break;
            case 'S':; case 's':;
            case KEY_DOWN:  if (item.line < GAME_HEIGHT - 1) item.line += 1; break;
            case 'D':; case 'd':;
            case KEY_RIGHT: if (item.column + FROG_DIM < GAME_WIDTH - 2) item.column += STEP; break;
            case 'A':; case 'a':;
            case KEY_LEFT:  if (item.column > 1)  item.column-= STEP; break;
        }
        writeItem(&buffer, &item, MAIN_PIPE);
    }
}

void moveFrog(Buffer* buffer, Item item, int direction) {
    switch (direction) {
        case RIGHT: if (item.column + FROG_DIM < GAME_WIDTH - 2) item.column += STEP; break;
        case LEFT:  if (item.column > 1) item.column -= STEP; break;
    }
    writeItem(buffer, &item, REVERSE_PIPE);
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

void timer(Buffer buffer, Item item) {
    while(1) {
        usleep(item.speed * USLEEP);
        if (item.column > 1 ) item.column -= 1;
        else item.type = LOSE;
        writeItem(&buffer, &item, MAIN_PIPE);
    }
}