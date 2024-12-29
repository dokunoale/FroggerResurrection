#include "entities.h"

void frog (Buffer *buffer, Item *item) {

    while (TRUE) {
        int c = getch();
        switch (c) {
            case 'q': item->type = EXIT; break;
            case KEY_UP: if (item->line > 1) item->line -= 1; break;
            case KEY_DOWN: if (item->line < GAME_HEIGHT - 2) item->line += 1; break;
            case KEY_RIGHT: if (item->column < GAME_WITDH - 3) item->column += 1; break;
            case KEY_LEFT: if (item->column > 2)  item->column-= 1; break;
        }

        writeItem(buffer, item);
    }
}

void crocodile (Buffer *buffer, Item *item, Game *game) {
    while(TRUE){
    
        switch (game->flows[item->line].direction) //vede la direzione del flusso dove sta il coccodrillo
        {
        case RIGHT: item->column += 1;
            break;
        case LEFT: item->column -= 1;
            break;
        }
        writeItem(buffer, item);
        usleep(item->speed * USLEEP);
    }
}