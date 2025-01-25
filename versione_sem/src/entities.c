#include "entities.h"
#include "utils.h"
#include "display.h"

void frog(Buffer buffer, Item item) {
    Item received;
    while(1) {
        writeItem(&buffer, &item, MAIN_BUF);
        usleep(2000);
        item.type = FROG;
        int c; 
        int size = readItem(&buffer, &received, REVERSE_BUF);
        if (size != 0) { 
            if (received.line == item.line) {
                item = received;
                // writeItem(&buffer, &item, MAIN_BUF); 
                continue; 
            }
        } 
        
        if (input(&c) == 0) continue;
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
        // writeItem(&buffer, &item, MAIN_BUF);
    }
}

void moveFrog(Buffer* buffer, Item item, int direction) {
    switch (direction) {
        case RIGHT: if (item.column + FROG_DIM < GAME_WIDTH - 2) item.column += STEP; break;
        case LEFT:  if (item.column > 1) item.column -= STEP; break;
    }
    writeItem(buffer, &item, REVERSE_BUF);
}

void crocodile(Buffer buffer, Item item) {
    while(1){
        switch(item.direction) {
            case RIGHT: item.column += STEP; break;
            case LEFT:  item.column -= STEP; break;
        }
        if ( item.column % STAGE_SKIP == 0 ) { item.stage = (item.stage + 1) % (STAGE_NUM); }
        writeItem(&buffer, &item, MAIN_BUF);
        usleep(item.speed * USLEEP);
    }
}

void bullet(Buffer buffer, Item item) {
    while(1){
        switch(item.direction) {
            case RIGHT: item.column += STEP; break;
            case LEFT:  item.column -= STEP; break;
        }
        writeItem(&buffer, &item, MAIN_BUF);
        usleep(item.speed * USLEEP);
    }
}

void timer(Buffer buffer, Item item) {
    while(1) {
        usleep(item.speed * USLEEP);
        if (item.column > 1 ) item.column -= 1;
        else item.type = LOSE;
        writeItem(&buffer, &item, MAIN_BUF);
    }
}

